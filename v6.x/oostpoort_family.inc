<?php

/* 
**  Church Administration (oostpoort) module for drupal 
**
**  Copyright (C) 2009-2010
**  =======================
**
**  Created by wplaat
**
**  For more information visit the following website.
**  Website : http://www.plaatsoft.nl 
**
**  Or send an email to the following address.
**  Email   : info@plaatsoft.nl
**
**  This program is free software; you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, version 2.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

// ##################################################################################
// State Machine
// ##################################################################################

function oostpoort_family()  
{
	$Action=$_POST['Action'];
	if ($Action=="") {
		$Action=$_SESSION['Action'];
	}
	oostpoort_debug_param("Action", $Action);
	$_SESSION['Action']=$Action;
	        	  
	if (($Action=="FamilyView") or ($Action=="AddFamilyPicture")) {
	
     $page = oostpoort_family_view();
	  
	} else if ($Action=="MemberView") {
	
     $page = oostpoort_member_view();
	  
	} else if ($Action=="Save") {
	
	   // Get Post and/or Session variables
		$PrsId=$_POST['PrsId'];
		if ($PrsId=="") {
			$PrsId=$_SESSION['PrsId'];
		}
		$_SESSION['PrsId']=$PrsId;
		oostpoort_debug_param("PrsId", $PrsId);
	
	  $page  = oostpoort_member_save();
	  $page .= oostpoort_save_picture("Prs",$PrsId);
     $page .= oostpoort_member_view();
	  
	} else  {
	
	  $page .= oostpoort_family_list();
	
	}
	print theme("page", $page);
}
	
// ##################################################################################
// Views
// ##################################################################################

/*
 * Function shows all family in a list form
 */
function oostpoort_family_list() {

	$page .= '<div class="oostpoort">';

	$Search=$_POST['Search'];
	oostpoort_debug_param("Search", $Search);
	
	// Get Sort value 
	$Sort=$_POST['Sort'];
	if ($Sort=="") {
		$Sort="A";
	}
	oostpoort_debug_param("Sort", $Sort);
			
	// Breadcrumb menu
	$page  = '<div class="breadcrumb">';
	$page .= '<a href="'.url("").'">Start</a> > ';
	$page .= '<a href="'.url(OOSTPOORT_MENU).'">Oostpoort</a> > ';
	$page .= t('Leden Gegevens');
	$page .= '</div>';

	$page .= '<div class="oostpoort">';
	
	drupal_set_title(t('Leden Gegevens'));

	$page .= '<form name="OostpoortForm" method="POST" >';
	
	$page .= '<fieldset>' ;
	$page .= '<legend>'.t('Filteren').'</legend>';
	
	$page .= '<div class="form-item">';
	$page .= '<label>'.t('Achternaam').' :</label>';
	$page .= '<input id="text" name="Search" size="40" maxlength="40" type="text" value="'.$Search.'"> ';
	$page .= oostpoort_hiddenlink('OostpoortForm','','','Search',t('Zoek'));
	$page .= '<div class="description">';
	$page .= t('Voer een achternaam en druk op enter om direct een familie te zoeken.');
	$page .= '</div>';
	$page .= '</div>';
	
	$page .= oostpoort_sortbar($Sort);
	  
	$page .= '</fieldset>';
	
  	$query = 'select ';
	$query .= ' distinct b.Adr_id as AdrId, ';
	$query .= ' concat(a.Prs_Tussenvoegsels, " ", substring_index(a.Prs_Achternaam,"-",1)) as Naam, ';
	$query .= ' concat(b.Adr_Straat, " " ,b.Adr_Huisnr) as Straat, ';
	$query .= ' b.Adr_Plaats as AdrPlaats, ';
	$query .= ' b.Adr_Postcode as AdrPostcode, ';	
	$query .= ' b.Adr_Telefoon as AdrTelefoon ';	
	$query .= 'from ';
	$query .= '  oostpoort_persoon a, oostpoort_adres b ';
	$query .= 'where ';
	$query .= ' a.Prs_Huidig_Adr_ID=b.Adr_id and ';
	if ($Search=="") {
		$query  .= 'a.Prs_Achternaam LIKE "'.$Sort.'%" ';
	} else {
		$query  .= 'a.Prs_Achternaam LIKE "'.$Search.'%" ';
	}
	$query .= 'order by ';
	$query .= ' a.Prs_Achternaam';
   oostpoort_debug_sql($query);
	$queryResult = db_query($query);

	$page .= '<div class="clear-block">';
	$page .= '<table>';
	$page .= '<thead>';
	$page .= '<tr>';
	$page .= '<th><b>'.t('Foto').'</b></th>';
	$page .= '<th class="active"><b>'.t('Achternaam').'</b></th>';
	$page .= '<th><b>'.t('Straat').'</b></th>';
	$page .= '<th><b>'.t('Postcode').'</b></th>';
	$page .= '<th><b>'.t('Plaats').'</b></th>';
	$page .= '</tr>';
   $page .= '</thead>';
 
	$page .= '<tbody>';
	$page_tmp='';
	while ($data = db_fetch_object($queryResult))
	{
		if ((++$i%2)==0) {
			$page_tmp .= '<tr class="even">';
		} else {
			$page_tmp .= '<tr class="odd">';
		}
		$page_tmp .= '<td>'.oostpoort_load_picture("Adr",$data->AdrId, 25, 35, "left" ).'</td>';
		$page_tmp .= '<td class="active">'.oostpoort_hiddenlink('OostpoortForm', 'AdrId', $data->AdrId, 'FamilyView', $data->Naam)."</td>";
		$page_tmp .= '<td>'.$data->Straat.'</td>';
		$page_tmp .= '<td>'.$data->AdrPostcode.'</td>';
		$page_tmp .= '<td>'.$data->AdrPlaats.'</td>';
		$page_tmp .= '</tr>';
	}

	if ( $page_tmp!='') {
		$page .= $page_tmp;
	} else  {
		$page .= '<tr>';
		$page .= '<td>'.t('Geen informatie gevonden').'</td>';
		$page .= '</tr>';
	}
	$page .= '</tbody>';
	$page .= '</table>';

	$page .= '<a href="'.url(OOSTPOORT_MENU).'">'.t('Terug').'</a>';
	
	$page .= '</div>';
	$page .= '</form>';
	
   return $page;
}

/*
 * Function shows all family in a view form
 */
function oostpoort_family_view() {

    // Get Post and/or Session variables
	$AdrId=$_POST['AdrId'];
   if ($AdrId=="") {
		$AdrId=$_SESSION['AdrId'];
	}
	$_SESSION['AdrId']=$AdrId;
	oostpoort_debug_param("AdrId", $AdrId);
	
	$Action=$_POST['Action'];
	oostpoort_debug_param("Action", $Action);
				
	// Breadcrumb menu
	$page  = '<div class="breadcrumb">';
	$page .= '<a href="'.url("").'">Start</a> > ';
	$page .= '<a href="'.url(OOSTPOORT_MENU).'">Oostpoort</a> > ';
	$page .= t('Leden Gegevens');
	$page .= ' > ';
	$page .= t('Familie overzicht');
	$page .= '</div>';

	$page .= '<div class="oostpoort">';
	
	drupal_set_title(t('Familie overzicht'));

	$page .= '<form enctype="multipart/form-data" name="OostpoortForm" method="POST" >';
			
	$page .= oostpoort_save_picture("Adr",$AdrId);
					
	$query  = 'select ';
	$query .= ' concat(a.Adr_Straat, " " ,a.Adr_Huisnr) as straat, ';
	$query .= ' a.Adr_Plaats as AdrPlaats, ';
	$query .= ' a.Adr_Postcode as AdrPostcode, ';	
	$query .= ' a.Adr_Telefoon as AdrTelefoon ';	
	$query .= 'from ';
	$query .= ' oostpoort_adres a ';
	$query .= 'where ';
	$query .= ' a.Adr_Id='.$AdrId;
	oostpoort_debug_sql($query);
	$queryResult = db_query($query); 
	$data = db_fetch_object($queryResult);
		
	$page .= '<fieldset>' ;
	$page .= '<legend></legend>';

	$page .= oostpoort_load_picture("Adr",$AdrId, 0, 0,"right");
	
	$page .= '<h4>'.$data->straat.'</h4>';	
	$page .= '<h4>'.$data->AdrPostcode.'&nbsp;&nbsp;'.$data->AdrPlaats.'</h4>';	
	$page .= '<h4>'.t('Nederland').'</h4>';		
	$page .= '<h4>'.$data->AdrTelefoon.'</h4>';	
		
	$query  = 'select ';
	$query .= '  a.Prs_Id as PrsId, ';
	$query .= '  concat(a.Prs_Voorletters," ", a.Prs_Tussenvoegsels, " ", a.Prs_Achternaam) as Naam, ';
	$query .= '  a.Prs_GeslachtMV as PrsGeslacht, '; 
	$query .= '  a.Prs_GeboorteDatum as PrsGeboorteDatum, ';
	$query .= '  a.Prs_TelefoonMobiel as PrsMobiel, ';
	$query .= '  a.Prs_EmailAdres as PrsEmail ';  
	$query .= 'from ';
	$query .= '  oostpoort_persoon a ';
	$query .= 'where ';
	$query .= '  a.Prs_Huidig_Adr_ID='.$AdrId.' ';
	$query .= 'order by ';
	$query .= '  a.Prs_GeboorteDatum';
	oostpoort_debug_sql($query);
	$queryResult = db_query($query);
  
	// Show Banner
	$page .= '<table>';
	$page .= '<thead>';
	$page .= '<tr>';
	$page .= '<th><b>'.t('Foto').'</b></th>';
	$page .= '<th><b>'.t('Naam').'</b></th>';
	//$page .= '<th><b>'.t('Geslacht').'</b></th>';
	$page .= '<th class="active"><b>'.t('Geboren').'</b></th>';
	$page .= '<th><b>'.t('Mobiel').'</b></th>';
	$page .= '<th><b>'.t('Email').'</b></th>';	
	$page .= '</tr>';
	$page .= '</thead>';

	// Show all found members
	$page .= '<tbody>';
	$page_tmp='';
	while ($data = db_fetch_object($queryResult))
	{
		if ((++$i%2)==0) {
			$page .= '<tr class="even">';
		} else {
			$page .= '<tr class="odd">';
		}
		$page .= '<td>'.oostpoort_load_picture("Prs",$data->PrsId, 30, 25, "Left").'</td>';
		$page .= '<td>'.oostpoort_hiddenlink('OostpoortForm', 'PrsId', $data->PrsId, 'MemberView', $data->Naam)."</td>";
		//$page .= '<td>'.oostpoort_view_geslacht($data->PrsGeslacht).'</td>';
		$page .= '<td class="active">'.oostpoort_view_date($data->PrsGeboorteDatum).'</td>';
		$page .= '<td>'.$data->PrsMobiel.'</td>';
		$page .= '<td>'.oostpoort_view_email($data->PrsEmail).'</td>';
		$page .= '</tr>';
	}
	$page .= '</fieldset>';
	
	$page .= '</tbody>';
	$page .= '</table>';

   if ($Action=="AddFamilyPicture") { 
	 
		$page .= '<div class="form-item">';
		$page .= '<label>'.t('Foto bestand').':</label>';
		$page .= '<input name="uploadedfile" type="file" size="100" onChange="submit();" />';
		$page .= '<div class="description">Alleen jpg foto formaat wordt ondersteund en foto mag niet groter zijn dan '.ini_get('upload_max_filesize').'B</div>';
		$page .= '</div>';

	} else {
	
		$page .= oostpoort_hiddenlink('OostpoortForm','AdrId',$data->AdrId,'AddFamilyPicture',t('Foto')).' | ';
	}
	$page .= oostpoort_hiddenlink('OostpoortForm','','','List',t('Terug'));
	
   return $page;
}	


function oostpoort_member_view()
{   
    // Get Post and/or Session variables
	$PrsId=$_POST['PrsId'];
   if ($PrsId=="") {
		$PrsId=$_SESSION['PrsId'];
	}
	$_SESSION['PrsId']=$PrsId;
	oostpoort_debug_param("PrsId", $PrsId);

	$Action=$_POST['Action'];
	oostpoort_debug_param("Action", $Action);
	
	// Breadcrumb menu
	$page  = '<div class="breadcrumb">';
	$page .= '<a href="'.url("").'">Start</a> > ';
	$page .= '<a href="'.url(OOSTPOORT_MENU).'">Oostpoort</a> > ';
	$page .= t('Leden Gegevens');
	$page .= ' > ';
	$page .= t('Familie overzicht');
	$page .= ' > ';
	$page .= t('Lid overzicht');
	$page .= '</div>';

	$page .= '<div class="oostpoort">';
	
	drupal_set_title(t('Lid overzicht'));

	$page .= '<form enctype="multipart/form-data" name="OostpoortForm" method="POST" >';
					
	// Query
	$query  = 'select ';
	$query .= ' concat(a.Prs_Voornamen," ",a.Prs_Tussenvoegsels," ",a.Prs_Achternaam) as Naam, ';
	$query .= ' a.Prs_GeslachtMV as PrsGeslacht, '; 
	$query .= ' a.Prs_Roepnaam as PrsRoepnaam, ';
	$query .= ' a.Prs_EmailAdres as PrsEmail, ';
	$query .= ' a.Prs_TelefoonMobiel as PrsMobiel, ';
	$query .= ' a.Prs_Geboortedatum as PrsGeboorteDatum, ';
	$query .= ' a.Prs_Huwelijksdatum as PrsHuwelijksDatum, ';
	$query .= ' a.Prs_KringlidJN as PrsKringLid, ';
	$query .= ' a.Prs_BezoekInfo as PrsBezoekInfo, ';
	$query .= ' concat(b.Adr_Straat, " " ,b.Adr_Huisnr) as straat, ';
	$query .= ' b.Adr_Plaats as AdrPlaats, ';
	$query .= ' b.Adr_Postcode as AdrPostcode, ';	
	$query .= ' b.Adr_Telefoon as AdrTelefoon, ';
	$query .= ' b.Adr_Id as AdrId ';
	$query .= 'from ';
	$query .= ' oostpoort_persoon a, oostpoort_adres b ';
	$query .= 'where ';
	$query .= ' a.Prs_Huidig_Adr_ID=b.Adr_ID and '; 
	$query .= ' a.Prs_ID = '.$PrsId;
	oostpoort_debug_sql($query);	
	$queryResult = db_query($query);
	$data = db_fetch_object($queryResult);
		
	$page .= '<div class="oostpoort">';
	
	$page .= '<fieldset>' ;
	$page .= '<legend>'.t('Kerkelijke bureau gegevens').'</legend>';
	$page .= oostpoort_load_picture("Prs",$PrsId,0,0,"right");
	$page .= '<h4>'.$data->Naam.'</h4>';	
	$page .= '<h4>'.$data->straat.'</h4>';	
	$page .= '<h4>'.$data->AdrPostcode.'&nbsp;&nbsp;'.$data->AdrPlaats.'</h4>';	
	$page .= '<h4>'.t('Nederland').'</h4>';	

	$page .= '<div class="form-item">';
	$page .= '<label>'.t('Geslacht').':</label>';
	$page .= oostpoort_view_geslacht($data->PrsGeslacht);
	$page .= '</div>';
	
	$page .= '<div class="form-item">';
	$page .= '<label>'.t('Geboorte datum').':</label>';
	$page .= oostpoort_view_date($data->PrsGeboorteDatum);
	$page .= '</div>';

	$page .= '<div class="form-item">';
	$page .= '<label>'.t('Huwelijksdatum').':</label>';
	$page .= oostpoort_view_date($data->PrsHuwelijksDatum);
	$page .= '</div>';
	
	$page .= '</fieldset>' ;
	
	$page .= '<fieldset>' ;
	$page .= '<legend>Extra gegevens</legend>';
		
	$page .= '<div class="form-item">';
	$page .= '<label>'.t('Roepnaam').':</label>';
	$page .= '<input id="text" name="FirstName" size="20" maxlength="20" type="text" value="'.$data->PrsRoepnaam.'">';
	$page .= '<div class="description">';
	$page .= t('Roepnaam van persoon.');
	$page .= '</div>';
	$page .= '</div>';

	$page .= '<div class="form-item">';
	$page .= '<label>'.t('Telefoon').':</label>';
	$page .= '<input id="text" name="Phone" size="20" maxlength="20" type="text" value="'.$data->AdrTelefoon.'">';
	$page .= '<div class="description">';
	$page .= t('Telefoon nummer van persoon.');
	$page .= '</div>';
	$page .= '</div>';
	
	$page .= '<div class="form-item">';
	$page .= '<label>'.t('Mobiel').':</label>';
	$page .= '<input id="text" name="Mobile" size="20" maxlength="20" type="text" value="'.$data->PrsMobiel.'">';
	$page .= '<div class="description">';
	$page .= t('Mobiele nummer van persoon.');
	$page .= '</div>';
	$page .= '</div>';

	$page .= '<div class="form-item">';
	$page .= '<label>'.t('Email').':</label>';
	$page .= '<input id="text" name="Email" size="60" maxlength="60" type="text" value="'.$data->PrsEmail.'">';
	$page .= '<div class="description">';
	$page .= t('Email adres van persoon.');
	$page .= '</div>';
	$page .= '</div>';
	    	
	$page .= '<div class="form-item">';
	$page .= '<label>'.t('Kring lid').':</label>';
	$page .= oostpoort_view_checkbox($data->PrsKringLid, false);	  
	$page .= '<div class="description">';
	$page .= t('Actief op een bijbelkring, Alfa groep en/of groeigroep.');
	$page .= '</div>';
	$page .= '</div>';
	
	$page .= '<div class="form-item">';
	$page .= '<label>'.t('Notitie').':</label>';
	$page .= oostpoort_view_note("BezoekInfo", $data->PrsBezoekInfo, false);
	$page .= '<div class="description">';
	$page .= t('Hier kunt U algemene niet vertrouwelijke informatie vastleggen. Bv. Beroep, Hobby, etc...');
	$page .= '</div>';
	$page .= '</div>';
	
	$page .= '<div class="form-item">';
	$page .= '<label>'.t('Foto bestand').':</label>';
	$page .= '<input name="uploadedfile" size="100" type="file" />';
	$page .= '<div class="description">';
	$page .= t('Alleen jpg foto formaat wordt ondersteund en foto mag niet groter zijn dan ').ini_get('upload_max_filesize').'B';
	$page .= '</div>';
	$page .= '</div>';

	$page .= oostpoort_hiddenlink('OostpoortForm','PrsId',$PrsId,'Save',t('Opslaan')).' | ';
	$page .= oostpoort_hiddenlink('OostpoortForm','AdrId',$data->AdrId,'FamilyView',t('Terug'));
  
   $page .= '</fieldset>' ;  
	$page .= '</form>';  
	$page .= '</div>';
	return $page;
}	
		
// ##################################################################################
// Actions
// ##################################################################################

function oostpoort_member_save() {
	
	// Get Posted parameters
	$PrsId=$_POST["PrsId"];
	oostpoort_debug_param("PrsId", $PrsId);
	
	$page .= '<div class="messages status">';
   $page .= t('Wijzingen voor persoon '.$PrsId.' zijn opgeslagen.');
	$page .= '</div>';
		
	$firstName=$_POST["FirstName"];
	oostpoort_debug_param("FirstName", $firstName);

	$email=$_POST["Email"];
	oostpoort_debug_param("Email", $email);

	$phone=$_POST["Phone"];
	oostpoort_debug_param("Phone", $phone);
	
	$mobile=$_POST["Mobile"];
	oostpoort_debug_param("Mobile", $mobile);
	
	$active=$_POST["Active"];
	oostpoort_debug_param("Active", $active);

	$BezoekInfo=$_POST["BezoekInfo"];
	oostpoort_debug_param("BezoekInfo", $BezoekInfo);
	   			
	if ($active == 'on') { 
	    $active_token="WAAR";
	} else {
		$active_token="ONWAAR";
	}
	
	$query  = 'UPDATE oostpoort_persoon SET ';
	$query .= '  Prs_BezoekInfo="'.$BezoekInfo.'", ';
	$query .= '  Prs_KringlidJN="'.$active_token.'", ';
	$query .= '  Prs_Roepnaam="'.$firstName.'", ';
	$query .= '  Prs_EmailAdres="'.$email.'", ';
	$query .= '  Prs_TelefoonMobiel="'.$mobile.'" ';
	$query .= 'where ';
	$query .= '  Prs_ID='.$PrsId;
	oostpoort_debug_sql($query);
	db_query($query); 
	
	$query  = 'select ';
	$query .= '  Prs_Huidig_Adr_ID as AdrId ';
	$query .= 'from ';
	$query .= '  oostpoort_persoon ';
	$query .= 'where ';
	$query .= '  Prs_ID='.$PrsId;
	oostpoort_debug_sql($query);
	$queryResult = db_query($query);
	$data = db_fetch_object($queryResult);
				
	$query  = 'UPDATE oostpoort_adres SET ';
	$query .= '  Adr_Telefoon="'.$phone.'" ';
	$query .= 'where ';
	$query .= '  Adr_ID='.$data->AdrId;
	oostpoort_debug_sql($query);
	db_query($query);  
	
	return $page;	
}
	
// ##################################################################################
// THE END
// ##################################################################################