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
// Views
// ##################################################################################

function oostpoort_upload_csv() {

	global $user;

	//only administrators can access this module
	if (!user_access('access administration pages')) {
		return drupal_access_denied();;
	}
	
		// Breadcrumb menu
	$page  = '<div class="breadcrumb">';
	$page .= '<a href="'.url("").'">Start</a> > ';
	$page .= '<a href="'.url(OOSTPOORT_MENU).'">Oostpoort</a> > ';
	$page .= t('ledenbestand upload');
	$page .= '</div>';
	
	$page .= '<div class="oostpoort">';
	
	drupal_set_title(t('Ledenbestand upload'));

	$page .= '<form enctype="multipart/form-data" name="OostpoortForm" method="POST" >';
	
	$page .= oostpoort_save_csv();
		
	$page .= '<fieldset>' ;
	$page .= '<legend>'.t('Leden bestand upload').'</legend>';	
	$page .= '<div class="form-item">';
	$page .= '  <label>'.t('Ledenbestand').':</label>';
	$page .= '  <input name="uploadedfile" type="file" size="100" onChange="submit();" />';
	$page .= '  <div class="description">';
	$page .= t('Alleen csv formaat wordt ondersteund en het bestand mag niet groter zijn dan ').ini_get('upload_max_filesize').'B';
	$page .= '  </div>';
	$page .= '</div>';
	$page .= '</fieldset>' ;  
	
	$page .= '<a href="'.url(OOSTPOORT_MENU).'">'.t('Terug').'</a>';
	
	$page .= '</form>';  
	$page .= '</div>';
	
	print theme("page", $page);
}

// ##################################################################################
// Utils
// ##################################################################################

function oostpoort_save_csv()
{
	// Fetch Image upload in a HTTP request
	if ($_FILES['uploadedfile']['tmp_name']!='') 
	{
		if ( (strstr($_FILES['uploadedfile']['name'],'.csv')) || 
		     (strstr( $_FILES['uploadedfile']['name'],'.CSV')) ) {
	
			$page .= oostpoort_process_csv($_FILES['uploadedfile']['tmp_name']);
			
		} else {
		
			$page .= '<div class="error status">';
			$page .= '<ul>';
			$page .= '<li>';
			$page .= t('Alleen CSV bestand formaat wordt ondersteund.');
			$page .= '</li>';
			$page .= '</ul>';
			$page .= '</div>';
		
			$_FILES['uploadedfile']['name']='';
		}
	}
	return $page;
}

function oostpoort_process_csv($filename) {

	$line_counter=0;
	$csv_file_error=0;
	$address_insert_counter=0;

	//$page .= "Filename=".$filename;
	
	$fh = fopen($filename, 'r');
	
	// Header line, skip it!
	$theData = fgets($fh);
	$line_counter++;

	// Process Data
	$theData = fgets($fh);
	$line_counter++;
	while ($theData!='')
	{	
		list( 
			$Status,
			$Selectiewijk,
			$Selectiesectie,
			$Wijknaam,
			$Geslacht,
			$Aanschrijfnaam,
			$VoorvoegselNaam,
			$Voornamen,
			$Straatnaam,
			$Huisnr,
			$HuisnrToev,
			$HuisLetter,
			$Postcode,
			$Woonplaats,
			$Leeftijd,
			$GeboorteDatum,
			$GeboortePlaats,
			$Burgelijkstaat,
			$KerkelijkeStaat,
			$HuwelijkDatum,
			$HuwelijkPlaats,
			$DoopDatum,
			$DoopPlaats,
			$KerkGezindte,
			$BelijdenisDatum,
			$BelijdenisPlaats,
			$HerkomstDatum,
			$HerkomstPlaats,
			$MutatieDatum,
			$KerkRelatie,
			$KerkBlad,
			$KerkLidNr) = split(";", $theData, 23);
	 
		if (oostpoort_update_adress($Straatnaam, $Huisnr.$HuisLetter, $Postcode, $Woonplaats)) {
			
			$address_insert_counter++;
		}
		
		if (oostpoort_update_member($Voornamen, $VoorvoegselNaam, $Aanschrijfnaam, $Geslacht,
											$GeboorteDatum, $HuwelijkDatum, $KerkLidNr)) {
			
			$member_insert_counter++;
		}
					
		$line_counter++;
		$theData = fgets($fh);
	}
	fclose($fh);

	if ($error==0)
	{
		$page .= '<div class="messages status">';
		$page .= '<ul>';
		$page .= '<li>';
		$page .= t('Ledenbestand is correct verwerkt.');
		$page .= '</li>';
		$page .= '<li>';
		$page .= ($line_counter-1).' '.t('leden verwerkt.');
		$page .= '</li>';
		$page .= '<li>';
		$page .= $address_insert_counter.' '.t('adressen toegevoegd.');
		$page .= '</li>';
		$page .= '</ul>';
		$page .= '</div>';
	}

	return $page;
}

function oostpoort_update_adress($Straatnaam,$HuisNr,$Postcode,$Woonplaats) {
	
	if (($HuisNr!="") && ($Postcode!="")) {
	
		$sql  = 'select ';
		$sql .= '  Adr_ID as AdrId ';
		$sql .= 'from ';
		$sql .= '  oostpoort_adres ';
		$sql .= 'where ';
		$sql .= '  Adr_Postcode="'.$Postcode.'" and ';
		$sql .= '  Adr_Huisnr="'.$HuisNr.'"';
 
		//oostpoort_debug_sql($sql);
		$queryResult = db_query($sql);     
		$data = db_fetch_object($queryResult);
	
		if ($data->AdrId=="") {
	
			$sql  = 'INSERT INTO oostpoort_adres ( ';
			$sql .= '`Adr_Straat`, ';
			$sql .= '`Adr_Huisnr`, ';
			$sql .= '`Adr_Plaats`, ';
			$sql .= '`Adr_Postcode` ';
			$sql .= ') VALUES (';
			$sql .= '"'.$Straatnaam.'", ';
			$sql .= '"'.$HuisNr.'", ';
			$sql .= '"'.$Woonplaats.'", ';
			$sql .= '"'.$Postcode.'") ';
		
			//oostpoort_debug_sql($sql);
			db_query($sql); 	
			
			return true;
		}
	}
	return false;
}


function oostpoort_update_member(
			$Voornamen,
			$VoorvoegselNaam,
			$Aanschrijfnaam,
			$Geslacht,
			$GeboorteDatum,
			$HuwelijkDatum,
			$KerkLidNr)  {
			
	if ($KerkLidNr!="")
	
		$sql  = 'select ';
		$sql .= '  Prs_ID as PrsId ';
		$sql .= 'from ';
		$sql .= '  oostpoort_persoon ';
		$sql .= 'where ';
		$sql .= '  Prs_Kerklidnummer="'.$KerkLidNr.'"';
 
		//oostpoort_debug_sql($sql);
		$queryResult = db_query($sql);     
		$data = db_fetch_object($queryResult);
		
		list( $Voorletters ) = split(" ", $Aanschrijfnaam, 1);
		
		$tmp=substr($Aanschrijfnaam, strlen($Voorletters));
				
		if ($data->PrsId=="") {
	
			$sql  = 'INSERT INTO oostpoort_adres ( ';
			$sql .= '`Prs_Kerklidnummer`, ';
			$sql .= '`Prs_Voorletters`, ';
			$sql .= '`Prs_Tussenvoegsels`, ';
			$sql .= '`Prs_Voornamen`, ';
			$sql .= '`Prs_Achternaam`, ';
			$sql .= '`Prs_GeslachtMV`, ';
			$sql .= '`Prs_Gem_ID`, ';
			$sql .= '`Prs_Bst_ID`, ';
			$sql .= '`Prs_Huidig_Adr_ID`, ';
			$sql .= '`Prs_Geboortedatum`, ';
			$sql .= '`Prs_Huwelijksdatum` ';
			$sql .= ') VALUES (';
			$sql .= '"'.$KerkLidNr.'", ';
			$sql .= '"'.$Voorletters.'", ';
			$sql .= '"'.$VoorvoegselNaam.'", ';
			$sql .= '"'.$Postcode.'") ';		
			if ($Geslacht=='Man') $sql .= '"M", '; else $sql .= '"V", ';
	}
}

function oostpoort_member_state($type) {
	
	$sql = 'select ';
	$sql = ' Bst_ID as BstId ';
	$sql = 'where ';
	$sql = ' Bst_Code=="'.$type.'"';
  
	//oostpoort_debug_sql($sql);
	$queryResult = db_query($sql);     
	$data = db_fetch_object($queryResult);
	
	return $data->BstId;
}

// ##################################################################################
// THE END
// ##################################################################################