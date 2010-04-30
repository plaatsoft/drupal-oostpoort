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
	$action=$_POST['action'];
	if ($action=="") {
		$action=$_SESSION['action'];
	}
	//echo "[action=".$action."]";
	$_SESSION['action']=$action;
	        	  
	if (($action=="FamilyView") or ($action=="AddFamilyPicture")) {
	
     $page .= oostpoort_family_view();
	  
	} else if ($action=="MemberView") {
	
     $page .= oostpoort_member_view();
	  
	} else if ($action=="Save") {
	
	   // Get Post and/or Session variables
		$PrsId=$_POST['PrsId'];
		if ($PrsId=="") {
			$PrsId=$_SESSION['PrsId'];
		}
		$_SESSION['PrsId']=$PrsId;
		//echo "[PrsId=".$PrsId."]";
	
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

	// Get search value out post or session scope
	$Search=$_POST['Search'];
	//echo "[Search=".$Search."]";
	
	// Get sort value 
	$Sort=$_POST['Sort'];
	if ($Sort=="") {
		$Sort=$_SESSION['Sort'];
		if ($Sort=="") {
			$Sort="A";
		}
	}
	$_SESSION['Sort']=$Sort;
	//echo "[Sort=".$Sort."]";
		
	// Breadcrumb menu
	$page  = '<div class="breadcrumb">';
	$page .= '<a href="'.url("").'">Start</a> > ';
	$page .= '<a href="'.url(OOSTPOORT_MENU).'">Oostpoort</a> > ';
	$page .= t('Adressen Lijst');
	$page .= '</div>';

	$page .= '<div class="oostpoort">';
	
	drupal_set_title(t('Oostpoort Familie overzicht'));

	$page .= '<br/>';

	$page .= '<form name="OostpoortForm" method="POST" >';
	
	$page .= '<input id="text" name="Search" size="40" maxlength="40" type="text" value="'.$search.'" ';
	$page .= '<input type="submit" value="'.t('Zoeken').'" />';

	$page .= '<br/>';
	
	$page .= oostpoort_sortbar($Sort);
	  
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
	if (strlen($search)==0) {
		$query  .= 'a.Prs_Achternaam LIKE "'.$Sort.'%" ';
	} else { 
		$query  .= 'a.Prs_Achternaam LIKE "'.$search.'%" ';
   }
	$query .= 'order by ';
	$query .= ' a.Prs_Achternaam';
	
  
	$queryResult = db_query($query);

	$page .= '<div class="clear-block">';
	$page .= '<table>';
	$page .= '<thead>';
	$page .= '<tr>';
	$page .= '<th><b>'.t('Foto').'</b></th>';
	$page .= '<th class="active"><b>'.t('Naam').'</b></th>';
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
		$page .= '<td></td>';
		$page .= '<td></td>';
		$page .= '<td></td>';
		$page .= '<td></td>';
		$page .= '<td>';
		$page .= '</tr>';
	}
	
	$page .= '<a href="'.url(OOSTPOORT_MENU).'">'.t('Terug').'</a>';
	
	$page .= '</tbody>';
	$page .= '</table>';
	$page .= '</div>';
	
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
	//echo "[AdrId=".$AdrId."]";
	
	$action=$_POST['action'];
	//echo "[Action=".$action."]";
				
	// Breadcrumb menu
	$page  = '<div class="breadcrumb">';
	$page .= '<a href="'.url("").'">Start</a> > ';
	$page .= '<a href="'.url(OOSTPOORT_MENU).'">Oostpoort</a> > ';
	$page .= t('Adressen Lijst');
	$page .= '</div>';

	$page .= '<div class="oostpoort">';
	
	drupal_set_title(t('Oostpoort Familie overzicht'));

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
  
	//echo $query;  
	$queryResult = db_query($query); 
	$data = db_fetch_object($queryResult);
		
	$page .= '<fieldset>' ;
	$page .= '<legend></legend>';

	$page .= oostpoort_load_picture("Adr",$AdrId, 0, 0,"right");
	
	$page .= '<h4>'.$data->straat.'</h4>';	
	$page .= '<h4>'.$data->AdrPlaats.' '.$data->AdrPostcode.'</h4>';		
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
			
	$queryResult = db_query($query);
	//echo $query;
  
	// Show Banner
	$page .= '<table>';
	$page .= '<thead>';
	$page .= '<tr>';
	$page .= '<th><b>'.t('Foto').'</b></th>';
	$page .= '<th><b>'.t('Naam').'</b></th>';
	$page .= '<th><b>'.t('Geslacht').'</b></th>';
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
		$page .= '<td>'.oostpoort_visit_geslacht($data->PrsGeslacht).'</td>';
		$page .= '<td class="active">'.oostpoort_view_date($data->PrsGeboorteDatum).'</td>';
		$page .= '<td>'.$data->PrsMobiel.'</td>';
		$page .= '<td>'.$data->PrsEmail.'</td>';
		$page .= '</tr>';
	}
	$page .= '</fieldset>';
	
	$page .= '</tbody>';
	$page .= '</table>';

   if ($action=="AddFamilyPicture") { 
	 
		$page .= '<div class="form-item">';
		$page .= '<label>'.t('Foto bestand').':</label>';
		$page .= '<input name="uploadedfile" type="file" onChange="submit();" />';
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
	//echo "[PrsId=".$PrsId."]";

	$action=$_POST['action'];
	//echo "[Action=".$action."]";
	
	// Breadcrumb menu
	$page  = '<div class="breadcrumb">';
	$page .= '<a href="'.url("").'">Start</a> > ';
	$page .= '<a href="'.url(OOSTPOORT_MENU).'">Oostpoort</a> > ';
	$page .= t('Adressen Lijst');
	$page .= '</div>';

	$page .= '<div class="oostpoort">';
	
	drupal_set_title(t('Oostpoort Familie overzicht'));

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
	
	$queryResult = db_query($query);
	$data = db_fetch_object($queryResult);
		
	$page .= '<div class="oostpoort">';
	
	$page .= '<fieldset>' ;
	$page .= '<legend>Persoon</legend>';
	$page .= oostpoort_load_picture("Prs",$PrsId,0,0,"right");
	$page .= '<h4>'.$data->Naam.'</h4>';	
	$page .= '<h4>'.$data->straat.'</h4>';	
	$page .= '<h4>'.$data->AdrPlaats.' '.$data->AdrPostcode.'</h4>';	

	$page .= '<div class="form-item">';
	$page .= '<label>'.t('Geslacht').':</label>';
	$page .= oostpoort_visit_geslacht($data->PrsGeslacht);
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
	$page .= '<legend>Extra Gegevens</legend>';
		
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
	$page .= '<input name="uploadedfile" type="file" />';
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
   //echo "[PrsId=".$PrsId."]";
	
	$page .= '<div class="messages status">';
   $page .= t('Wijzingen voor persoon '.$PrsId.' zijn opgeslagen.');
	$page .= '</div>';
		
	$firstName=$_POST["FirstName"];
   //echo "[firstName=".$firstName."]";

	$email=$_POST["Email"];
   //echo "[email=".$email."]";

	$phone=$_POST["Phone"];
   //echo "[phone=".$phone."]";
	
	$mobile=$_POST["Mobile"];
   //echo "[mobile=".$mobile."]";	
	
	$active=$_POST["Active"];
   //echo "[active=".$active."]";

	$BezoekInfo=$_POST["BezoekInfo"];
   //echo "[BezoekInfo=".$BezoekInfo."]";
	   			
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
	//echo $query;
	db_query($query); 
	
	$query  = 'select ';
	$query .= '  Prs_Huidig_Adr_ID as AdrId ';
	$query .= 'from ';
	$query .= '  oostpoort_persoon ';
	$query .= 'where ';
	$query .= '  Prs_ID='.$PrsId;
	//echo $query;
	$queryResult = db_query($query);
	$data = db_fetch_object($queryResult);
				
	$query  = 'UPDATE oostpoort_adres SET ';
	$query .= '  Adr_Telefoon="'.$phone.'" ';
	$query .= 'where ';
	$query .= '  Adr_ID='.$data->AdrId;
	//echo $query;
	db_query($query);  
	
	return $page;	
}

// ##################################################################################
// Utils
// ##################################################################################

				
function oostpoort_sortbar($sort)
{
	$sort_bar=array("A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P", "Q","R","S","T","U","V","W","X","Y","Z");
	for ($i=0; $i<26; $i++)
	{
		if ($sort==$sort_bar[$i]) $page .= '<B>';
		$page .= oostpoort_hiddenlink('OostpoortForm', 'Sort', $sort_bar[$i], '', $sort_bar[$i]).'|';
		if ($sort==$sort_bar[$i]) $page .= '</B>';
	}
	return $page;
}


function oostpoort_load_picture($Group,$AdrId,$height,$width, $align) {
	
	// Construct a valid drupal path for the named file
	$file=OOSTPOORT_THUMBNAILS_DIR.'/'.$Group.$AdrId.'.jpg';
   $target = file_create_path($file);

   // Check to see if the file exists
   if (file_exists($target)) {	
		$filename=str_replace(array("?q="), "", url($file));
		$page.='<img align="'.$align.'" ';
		if ($width!=0) {
			$page.='width="'.$width.'" ';
		}
		if ($height!=0) {
			$page.='height="'.$height.'" ';
		}
		$page.='src="'.$filename.'" />';
	} 
	return $page;
}
		
function oostpoort_save_picture($Group,$AdrId)
{
	// Fetch Image upload in a HTTP request
	if ($_FILES['uploadedfile']['name']!='') 
	{
		if ( (strstr($_FILES['uploadedfile']['name'],'.jpg')) || (strstr( $_FILES['uploadedfile']['name'],'.JPG')) )
		{
			$error='0';
			if(!is_dir(OOSTPOORT_IMAGE_DIR)) {
				if( !mkdir(OOSTPOORT_IMAGE_DIR) ) {
					$page .= '<div class="error status">';
					$page .= t('Creating image directory failed!');
					$page .= '</div>';
					$error='1';
				}
			}
			$filename= $Group.$AdrId.'.jpg';

			// Check dimensions of image
			$dimensions = getimagesize($_FILES['uploadedfile']['tmp_name']);
			$width  = $dimensions[0];
			$height = $dimensions[1];
			$resize = 0;

			// Limit picture 
			if ($width>$height) {
			
				if ($width>0) {
					$ratio = 800 / $width;
				}
				
				if ($ratio < 1) {
					$width=800;
					$height=$height*$ratio;
					$resize = 1;
				}
			} else  {
				if ($width>0) {
					$ratio = 600 / $width;
				}
				if ($ratio < 1) {
					$width=600;
					$height=$height*$ratio;
					$resize = 1;
				}
			}

			// Resize uploaded picture
			if ( $resize == 1 ) {
				if (!image_scale($_FILES['uploadedfile']['tmp_name'], file_create_path(OOSTPOORT_IMAGE_DIR.'/'.$filename), $width, $height)) {
					$page .= '<div class="error status">';
					$page .= t('Foto bestand te groot!');
					$page .= '</div>';
					$error='1';
				}
			} else {
				// Not resize needed, move file to storage place
				copy($_FILES['uploadedfile']['tmp_name'],OOSTPOORT_IMAGE_DIR.'/'.$filename);
			}

			if ($error==0)	{
				// Create thumbnails directory store
				if(!is_dir(OOSTPOORT_THUMBNAILS_DIR)) {
					if( !mkdir(OOSTPOORT_THUMBNAILS_DIR) )	{
						$page .= '<div class="error status">';
						$page .= t('Creating image directory failed!');
						$page .= '</div>';
						$error='1';
					}
				}

				$resize = 0;

				// Limit picture size on screen
				if ($width>$height) {
					if ($width>0) $ratio  = 160 / $width;
					if ($ratio < 1) {
						$width=160;
						$height=$height*$ratio;
						$resize = 1;
					}
				} else {
					if ($width>0) $ratio  = 120 / $width;
					if ($ratio < 1) {
						$width=120;
						$height=$height*$ratio;
						$resize = 1;
					}
				}

				// Create thumbnail picture
				// Resize uploaded picture
				if ( $resize == 1 ) {
					if (!image_scale(OOSTPOORT_IMAGE_DIR.'/'.$filename, file_create_path(OOSTPOORT_THUMBNAILS_DIR.'/'.$filename), $width, $height)) {
						$page .= '<div class="error status">';
						$page .= t('Foto bestand is te groot ');
						$page .= '</div>';
			
						$error='1';
					}
				} else {
					// Not resize needed, move file to storage place
					copy(OOSTPOORT_IMAGE_DIR.'/'.$filename, OOSTPOORT_THUMBNAILS_DIR.'/'.$filename);
				}

				if ($error=='0') {
					$page .= '<div class="messages status">';
					$page .= t('Foto is opgeslagen.');
					$page .= '</div>';
				}
			}
		} else {
			$page .= '<div class="error status">';
			$page .= t('Alleen JPG foto formaat wordt ondersteund.');
			$page .= '</div>';
		
			$_FILES['uploadedfile']['name']='';
		}
	}
	return $page;
}
	
// ##################################################################################
// THE END
// ##################################################################################