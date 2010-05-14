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

function oostpoort_family($PrsId=0) {
	
	// *****************************
	// Process POST an SESSION variables
	// *****************************
	
	$Action=htmlentities($_POST['Action']);
	oostpoort_debug_param("Action", $Action);
	
	if ($PrsId==0) {
		// Standard request, following posted action.
		$PrsId=htmlentities($_POST['PrsId']);
		if ($PrsId=="") {
			$PrsId=$_SESSION['PrsId'];
		}
	} else {
		// Birthday request, go directly to MemberView.
		$Action="MemberView";
	}
	$_SESSION['PrsId']=$PrsId;
	oostpoort_debug_param("PrsId", $PrsId);
			
	// *****************************
	// Process statemachine
	// *****************************
	
	switch ($Action) {
   
		case 'FamilyView':
		case 'AddFamilyPicture':
		case 'SaveFamily':
			$page = oostpoort_family_view(false);
			break;
					
		case 'MemberEdit':
			$page = oostpoort_member_view(false);
			break;
		
		case 'MemberView':
			$page = oostpoort_member_view(true);
			break;
			
		case 'DeleteMember':	
			oostpoort_member_delete();
			$page .= oostpoort_family_view(false);
			break;
		
		case 'DeleteFamilyPicture':
			oostpoort_delete_picture();
			$page = oostpoort_family_view(false);
			break;
			
		case 'DeleteMemberPicture':
			oostpoort_delete_picture();
			$page = oostpoort_member_view(false);
			break;
			
		case 'SaveMember':	
			oostpoort_save_picture("Prs",$PrsId);
			$error = oostpoort_member_save();
			if ($error==0) {
				$page .= oostpoort_family_view(false);
			} else {
				$page .= oostpoort_member_view(false);
			}
			break;
			
		default:
			$page .= oostpoort_family_list();
			break;				
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
			
	// *****************************
	// Get posted Inputs
	// *****************************
	
	// Process parameters
	$Search=htmlentities($_POST['Search']);
	oostpoort_debug_param("Search", $Search);
	
	// Get Sort value 
	$Sort=htmlentities($_POST['Sort']);
	if ($Sort=="") {
		$Sort=$_SESSION['Sort'];
		if ($Sort=="") {
			$Sort="A";
		}
	}
	$_SESSION['Sort']=$Sort;
	oostpoort_debug_param("Sort", $Sort);

	// *****************************
	// Query
	// *****************************
	
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
	
	// *****************************
	// Create page
	// *****************************
	
	$page .= '<form name="OostpoortForm" method="POST" >';
	$page .= '<div class="oostpoort">';
		
	// Breadcrumb menu
	$breadcrumb["child1"] = oostpoort_hiddenlink('OostpoortForm','','','List',t('Leden Gegevens'));
	oostpoort_breadcrumb($breadcrumb);
	
	drupal_set_title(t('Leden Gegevens'));
	
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
	
	$page .= oostpoort_sortbar($Sort,"List");
	  
	$page .= '</fieldset>';
	
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
		$page_tmp .= '<td>'.oostpoort_load_picture("Adr",$data->AdrId, 25, 35, "left", false).'</td>';
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
function oostpoort_family_view($readonly) {

   // *****************************
	// Get posted Inputs
	// *****************************
	
	$AdrId=htmlentities($_POST['AdrId']);
    if ($AdrId=="") {
		$AdrId=$_SESSION['AdrId'];
	}
	$_SESSION['AdrId']=$AdrId;
	oostpoort_debug_param("AdrId", $AdrId);
	
	$Action=htmlentities($_POST['Action']);
	oostpoort_debug_param("Action", $Action);

	// *****************************
	// Create page
	// *****************************

	$page .= '<form enctype="multipart/form-data" name="OostpoortForm" method="POST" >';
	$page .= '<div class="oostpoort">';
	
	if (!$readonly) {
		// Breadcrumb menu
		$node["child1"] = l(t('Leden Gegevens'),OOSTPOORT_FAMILY);
		$node["child2"] = t('Familie overzicht');
		oostpoort_breadcrumb($node);

		drupal_set_title(t('Familie overzicht'));
	}
	oostpoort_save_picture("Adr",$AdrId);
						
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

	$page .= oostpoort_load_picture("Adr",$AdrId, 0, 0,"right", true);
	
	$page .= '<h4>'.$data->straat.'</h4>';	
	$page .= '<h4>'.$data->AdrPostcode.'&nbsp;&nbsp;'.$data->AdrPlaats.'</h4>';	
	$page .= '<h4>'.$data->AdrTelefoon.'</h4>';	
		
	$query  = 
		'
		select 
	 		a.Prs_Id as PrsId, 
			concat(a.Prs_Voorletters," ", a.Prs_Tussenvoegsels, " ", a.Prs_Achternaam) as Naam, 
	  		a.Prs_GeslachtMV as PrsGeslacht, 
	  		a.Prs_GeboorteDatum as PrsGeboorteDatum,
	  		a.Prs_Huwelijksdatum as PrsHuwelijksdatum,  
	  		a.Prs_TelefoonMobiel as PrsMobiel, 
	  		a.Prs_EmailAdres as PrsEmail  
		from 
	  		oostpoort_persoon a 
		where 
		  	a.Prs_Huidig_Adr_ID='.$AdrId.'
		order by 
	  		a.Prs_GeboorteDatum
	  	';	
	oostpoort_debug_sql($query);
	$queryResult = db_query($query);
  
	// Show Banner
	$page .= '<table>';
	$page .= '<thead>';
	$page .= '<tr>';
	$page .= '<th><b>'.t('Foto').'</b></th>';
	$page .= '<th><b>'.t('Naam').'</b></th>';	
	$page .= '<th class="active"><b>'.t('Geboren').'</b></th>';
	$page .= '<th><b>'.t('Huwelijk').'</b></th>';
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
		$page .= '<td>'.oostpoort_load_picture("Prs",$data->PrsId, 30, 25, "Left", false).'</td>';
		if ($readonly) {
			$page .= '<td>'.$data->Naam.'</td>';
		} else { 
			$page .= '<td>'.oostpoort_hiddenlink('OostpoortForm', 'PrsId', $data->PrsId, 'MemberEdit', $data->Naam)."</td>";
		}
		$page .= '<td class="active">'.oostpoort_convert_date($data->PrsGeboorteDatum).'</td>';
		$page .= '<td>'.oostpoort_convert_date($data->PrsHuwelijksdatum).'</td>';
		$page .= '<td>'.$data->PrsMobiel.'</td>';
		$page .= '<td>'.oostpoort_view_email($data->PrsEmail).'</td>';
		$page .= '</tr>';
	}	
	$page .= '</tbody>';
	$page .= '</table>';
		
	if (!$readonly) {
		if ($Action=="AddFamilyPicture") { 
			$page .= '<div class="form-item">';
			$page .= '<label>'.t('Foto bestand').':</label>';
			$page .= '<input name="uploadedfile" type="file" size="100" />';
			$page .= '<div class="description">Alleen jpg foto formaat wordt ondersteund en foto mag niet groter zijn dan '.ini_get('upload_max_filesize').'B</div>';
			$page .= '</div>';
			
			$page .= oostpoort_hiddenlink('OostpoortForm','AdrId',$AdrId,'SaveFamily',t('Opslaan'));
		} else {
			$page .= oostpoort_hiddenlink('OostpoortForm','AdrId',$AdrId,'AddFamilyPicture',t('Foto Toevoegen'));
			if (oostpoort_check_picture("Adr",$AdrId)) {
				$page .= ' | '.oostpoort_hiddenlink('OostpoortForm','AdrId',$AdrId,'DeleteFamilyPicture',t('Foto Verwijderen'));
			} 
		}
	}

	$page .= '</fieldset>';
	
	if (!$readonly) {
		$page .= oostpoort_hiddenlink('OostpoortForm','','','List',t('Terug'));
	}
		
	$page .= '</div>';
	$page .= '</form>';
	return $page;
}	

function oostpoort_member_view($readonly) {   

	// *****************************
	// Get posted Inputs
	// *****************************
	
	$PrsId=htmlentities($_POST['PrsId']);
   	if ($PrsId=="") {
		$PrsId=$_SESSION['PrsId'];
	}
	$_SESSION['PrsId']=$PrsId;
	oostpoort_debug_param("PrsId", $PrsId);
	
	// *****************************
	// Database query
	// *****************************
	
	$query  = 'select ';
	$query .= ' concat(a.Prs_Voornamen," ",a.Prs_Tussenvoegsels," ",a.Prs_Achternaam) as Naam, ';
	$query .= ' a.Prs_GeslachtMV as PrsGeslacht, '; 
	$query .= ' a.Prs_Roepnaam as PrsRoepnaam, ';
	$query .= ' a.Prs_EmailAdres as PrsEmail, ';
	$query .= ' a.Prs_TelefoonMobiel as PrsMobiel, ';
	$query .= ' a.Prs_Geboortedatum as PrsGeboorteDatum, ';
	$query .= ' a.Prs_GeboortePlaats as PrsGeboortePlaats, ';
	$query .= ' a.Prs_Huwelijksdatum as PrsHuwelijksDatum, ';
	$query .= ' a.Prs_Kerklidnummer as PrsKerklidnummer, ';
	$query .= ' a.Prs_KringlidJN as PrsKringLid, ';
	$query .= ' a.Prs_BezoekInfo as PrsBezoekInfo, ';
	$query .= ' (select Bst_Omschrijving from oostpoort_burgelijkestaat where Bst_ID=a.Prs_Bst_ID) as Burgelijkestaat, ';
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
	
	// *****************************
	// Create page
	// *****************************
	
	$page .= '<form enctype="multipart/form-data" name="OostpoortForm" method="POST" >';
	$page .= '<div class="oostpoort">';
		
	// Breadcrumb menu
	$node["child1"] = l(t('Leden Gegevens'),OOSTPOORT_FAMILY);
	$node["child2"] = t('Familie overzicht');
	$node["child3"] = t('Lid overzicht');
	oostpoort_breadcrumb($node);

	drupal_set_title(t('Lid overzicht'));
						
	$page .= '<fieldset>' ;	
	$page .= '<legend>'.t('Kerkelijke bureau gegevens').'</legend>';
	
	$page .= oostpoort_load_picture("Prs",$PrsId,0,0,"right", true);
	$page .= '<h4>'.$data->Naam.'</h4>';	
	$page .= '<h4>'.$data->straat.'</h4>';	
	$page .= '<h4>'.$data->AdrPostcode.'&nbsp;&nbsp;'.$data->AdrPlaats.'</h4>';	

	$page .= oostpoort_form_item(
					t('Persoon Id:'),
					$PrsId,
					"",
					$readonly);
							
	$page .= oostpoort_form_item(
					t('Kerkelijke lid nummer:'),
					$data->PrsKerklidnummer,
					"",
					$readonly);
							
	$page .= oostpoort_form_item(
					t('Geslacht:'),							
					oostpoort_view_geslacht($data->PrsGeslacht),
					"",
					$readonly);

	$tmp = oostpoort_convert_date($data->PrsGeboorteDatum);
	if (($tmp!="") && ($data->PrsGeboortePlaats!="")) {
		$tmp.= ' ('.$data->PrsGeboortePlaats.')';
	}
	
	$page .= oostpoort_form_item(
					t('Geboren:'),							
					$tmp,
					"",
					$readonly);
	
	$page .= oostpoort_form_item(
					t('Huwelijk:'),							
					oostpoort_convert_date($data->PrsHuwelijksDatum),
					"",
					$readonly);
								
	$page .= oostpoort_form_item(
					t('Burgelijkestaat:'),							
					$data->Burgelijkestaat,
					"",
					$readonly);
					
	$page .= '</fieldset>' ;
	
	$page .= '<fieldset>' ;
	$page .= '<legend>Extra gegevens</legend>';
		
	$page .= oostpoort_form_item(
					t('Roepnaam:'),							
					oostpoort_view_text("FirstName", 30, 30, $data->PrsRoepnaam, $readonly),
					t('Roepnaam van persoon.'),
					$readonly);
							
	$page .= oostpoort_form_item(
					t('Telefoon:'),							
					oostpoort_view_text("Phone", 15, 15, $data->AdrTelefoon, $readonly),
					t('Telefoonnummer van persoon.'),
					$readonly);

	$page .= oostpoort_form_item(
					t('Mobiel:'),							
					oostpoort_view_text("Mobile", 15, 15, $data->PrsMobiel, $readonly),
					t('Mobiele nummer van persoon.'),
					$readonly);
					
	$page .= oostpoort_form_item(
					t('Email:'),							
					oostpoort_view_text("Email", 60, 100, $data->PrsEmail, $readonly),
					t('Email adres van persoon.'),
					$readonly);
							
	$page .= oostpoort_form_item(
					t('Kring lid:'),							
					oostpoort_view_checkbox("Active", $data->PrsKringLid, $readonly),
					t('Actief op een bijbelkring, Alfa groep en/of groeigroep.'),
					$readonly);
	    	
	$page .= oostpoort_form_item(
					t('Notitie:'),							
					oostpoort_view_note("BezoekInfo", $data->PrsBezoekInfo, $readonly),
					t('Hier kunt U algemene niet vertrouwelijke informatie vastleggen. Bv. Beroep, Hobby, etc...'),
					$readonly);
	    	
	$page .= oostpoort_form_item(
					t('Pasfoto:'),							
					oostpoort_input_picture($readonly),
					t('Alleen jpg foto formaat wordt ondersteund en foto mag niet groter zijn dan ').ini_get('upload_max_filesize').'B',
					$readonly);
						
	if (! $readonly) {	 
		$page .= oostpoort_hiddenlink('OostpoortForm','PrsId',$PrsId,'SaveMember',t('Opslaan'));
		if (oostpoort_check_picture("Prs",$PrsId)) {
			$page .= ' | '.oostpoort_hiddenlink('OostpoortForm','PrsId',$PrsId,'DeleteMemberPicture',t('Foto Verwijderen'));
		} 
		if (user_access('access administration pages')) {
			$page .= ' | '.oostpoort_hiddenlink('OostpoortForm','PrsId',$PrsId,'DeleteMember',t('Persoon Verwijderen'));
		}		
	}
  
	$page .= '</fieldset>' ;

	if (! $readonly) {
		$page .= oostpoort_hiddenlink('OostpoortForm','AdrId',$data->AdrId,'FamilyView',t('Terug'));
	}	
	$page .= '</div>';
	$page .= '</form>';  
	
	
	return $page;
}	
		
// ##################################################################################
// Actions
// ##################################################################################

function oostpoort_delete_picture() {
	
	// *****************************
	// Get posted Inputs
	// *****************************
	
	$file="";
	
	if ( isset($_POST["PrsId"])) {
		
		$Id=htmlentities($_POST["PrsId"]);
		$file='Prs'.$Id.'.jpg';
		
	} else if (isset($_POST["AdrId"])) {
		
		$Id=htmlentities($_POST["AdrId"]);	
		$file='Adr'.$Id.'.jpg';
	}
	oostpoort_debug_param("Id", $Id);
	
	// *****************************
	// Action
	// *****************************
	
	if ($file!="") {
		
		$target = file_create_path(OOSTPOORT_IMAGE_DIR.'/'.$file);
		if (file_exists($target)) {	
			unlink($target);
		}
		
		$target = file_create_path(OOSTPOORT_THUMBNAILS_DIR.'/'.$file);
		if (file_exists($target)) {	
			unlink($target);
		}	
	
		// *****************************
		// Create message
		// *****************************
		
		$msg .= t('Foto').' '.$Id.' '.t('verwijderd');
		drupal_set_message($msg, 'status');
		watchdog('user', $msg);
	}
}

function oostpoort_member_save() {
	
	// *****************************
	// Get posted Inputs
	// *****************************
	
	$PrsId=htmlentities($_POST["PrsId"]);
	oostpoort_debug_param("PrsId", $PrsId);
	
	$firstName=htmlentities($_POST["FirstName"]);
	oostpoort_debug_param("FirstName", $firstName);

	$email=htmlentities($_POST["Email"]);
	oostpoort_debug_param("Email", $email);

	$phone=htmlentities($_POST["Phone"]);
	oostpoort_debug_param("Phone", $phone);
	
	$mobile=htmlentities($_POST["Mobile"]);
	oostpoort_debug_param("Mobile", $mobile);
	
	$active=htmlentities($_POST["Active"]);
	oostpoort_debug_param("Active", $active);

	$BezoekInfo=htmlentities($_POST["BezoekInfo"]);
	oostpoort_debug_param("BezoekInfo", $BezoekInfo);
	   			
	if ($active == 'on') { 
	    $active_token="WAAR";
	} else {
		$active_token="ONWAAR";
	}
	
	// *****************************
	// Database actions
	// *****************************
	
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
	
	// *****************************
	// Create message
	// *****************************
		
	$msg .= t('Wijzingen voor persoon '.$PrsId.' zijn opgeslagen.');
	drupal_set_message($msg, 'status');
	watchdog('user', $msg);
	
	return 0;	
}

function oostpoort_member_delete() {
	
	// *****************************
	// Get posted Inputs
	// *****************************
	
	$PrsId=htmlentities($_POST["PrsId"]);
	oostpoort_debug_param("PrsId", $PrsId);
	if ($PrsId!="") {
		return 1;
	}
	
	// *****************************
	// Database actions
	// *****************************
	
	$query = 'select Prs_Huidig_Adr_ID as AdrId FROM oostpoort_persoon WHERE prs_id='.$PrsId;
	oostpoort_debug_sql($query);	
	$queryResult = db_query($query);
	$data = db_fetch_object($queryResult);
	$AdrId=$data->AdrId;
	
	if ($AdrId!=0) {
		$query = 'delete FROM oostpoort_persoon WHERE prs_id='.$PrsId;		
		oostpoort_debug_sql($query);
		db_query($query);  
	
		// *****************************
		// Create message
		// *****************************
	
		$msg .= t('Persoon '.$PrsId.' is verwijderd.');
		drupal_set_message($msg, 'status');
		watchdog('user', $msg);
	}
	
	$_SESSION['AdrId']=$AdrId;	
	return 0;		
}

// ##################################################################################
// THE END
// ##################################################################################