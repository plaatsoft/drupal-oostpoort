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
// STATE MACHINE
// ##################################################################################

function oostpoort_visit()  
{
	if(isset($_SESSION['views']))
		$_SESSION['views'] = $_SESSION['views']+ 1;
	else
		$_SESSION['views'] = 1;
	echo "views = ". $_SESSION['views']; 

	$action=$_POST['action'];
	echo "[action=".$action."]";
    
	if ($action=="Edit") {
	
		$page .= oostpoort_visit_address_view();
	
	} else if ($action=="Cancel") {
		
		$page .= oostpoort_visit_list_view();
	
	} else if ($action=="Search") {

		$page .= oostpoort_visit_list_search();
	
	} else if ($action=="Select") {

		$page .= oostpoort_visit_select();
		$page .= oostpoort_visit_list_view();
	
	} else if ($action=="DeSelect") {
	
		$page .= oostpoort_visit_deselect();
		$page .= oostpoort_visit_list_view();
	
	} else if ($action=="Ready") {
	
		$page .= oostpoort_visit_ready();
		$page .= oostpoort_visit_list_view();
	
	} else if ($action=="MainMenu") {
	
		oostpoort_mainmenu();
		$page="";
	
	} else {

		$page = oostpoort_visit_list_view();
	}

   if ($page!="") {
		print theme("page", $page);
	}
}

// ##################################################################################
// PAGES
// ##################################################################################

function oostpoort_visit_list_view() {
  
	// Create Breadcrumb menu
	$page  = '<div class="breadcrumb">';
	$page .= '<a href="'.url("").'">Start</a> > ';
	$page .= '<a href="'.url(OOSTPOORT_MENU).'">Oostpoort</a> > ';
	$page .= 'Visit > List';
	$page .= '</div>';

   // Get posted parameters
	$PrsId=$_POST["PrsId"];
	if ($PrsId=="") {
	   $PrsId=$_SESSION["PrsId"];
		if ($PrsId=="") {
			$PrsId=0;
		}
	}	
	echo "[PrsId=".$PrsId."]";
	
	// Set Session scope variables
	$_SESSION["PrsId"]=$PrsId;
	
	// Build page
	$page .= '<div class="oostpoort">';
	
	drupal_set_title(t('Oostpoort bezoekwerk' ) );

	$page .= '<form name="OostpoortForm" method="POST" >';
     
	$query  = 'select ';
	$query .= '  concat(IF(b.Prs_Roepnaam="",b.Prs_Voornamen, b.Prs_Roepnaam)," ", b.Prs_Tussenvoegsels, " ", b.Prs_Achternaam) as Naam, '; 
	$query .= '  b.Prs_id as PrsId, ';
	$query .= '  a.Mdw_ActiefJN as Actief ';
	$query .= 'from ';
	$query .= '  oostpoort_medewerker a, oostpoort_persoon b ';
	$query .= 'where ';
	$query .= ' a.Mdw_Prs_id=b.Prs_id ';
	$query .= 'order by ';
	$query .= ' Naam';
  
	//echo $query;  
	$queryResult = db_query($query); 
  
   $page .= '<br/>';
	$page .= '<b>Bezoekmedewerker </b>';  
	$page .= '<select name="PrsId" onChange="submit();">';   
	while ($data = db_fetch_object($queryResult))
	{
		$page .= '<option value="'.$data->PrsId.'" ';
		if ($data->PrsId==$PrsId) $page.='selected="selected" ';
		$page .= '>'.$data->Naam.' ['.$data->PrsId.']';
		if ($data->Actief=='ONWAAR') $page .= ' (Niet Actief)';
		$page .= '</option>';
	}
	$page .='</select> '; 
	$page .= oostpoort_hiddenlink('OostpoortForm','',0,'List',t('Zoeken'));
  
   $page .= '<br/>';
	$page .= '<br/>';
   $page .= t('Dit zijn alle ingeboekte bezoeken voor de geselecteerde bezoekmedewerker.');
  
	// Query Database
	$query  = 'select ';
	$query .= '   concat (c.Adr_Straat," ", c.Adr_Huisnr) as Straatnaam, ';
	$query .= '   c.Adr_Plaats as plaats, ';
	$query .= '   c.Adr_postcode as postcode, ';
	$query .= '   a.Bzk_GeplandDatum as geplande, ';
	$query .= '   a.Bzk_AfgelegdDatum as afgelegd, ';
	$query .= '   a.Bzk_ID as BzkId '; 
	$query .= 'from ';
	$query .= '   oostpoort_bezoek a, oostpoort_adres c ';
	$query .= 'where ';
	$query .= '   a.Bzk_Adr_ID=c.Adr_id and ';
	$query .= '   a.Bzk_AfgelegdDatum=0 and ';
	$query .= '   a.Bzk_GeannuleerdDatum=0 and ';
   $query .= '   a.Bzk_BezoekNietGewenstDatum=0 and '; 
	$query .= '   a.Bzk_Mdw_id='.$PrsId.' ';
	$query .= 'order by ';
	$query .= '   Straatnaam';
  
	$queryResult = db_query($query);
	//echo $query;
  
	// Show Banner
	$page .= '<table>';
	$page .= '<tr>';
	$page .= '<thead>';
	$page .= '<tr>';
	$page .= '<th><b>'.t('Rij').'</b></th>';
	$page .= '<th><b>'.t('Straat').'</b></th>';
	$page .= '<th><b>'.t('Postcode').'</b></th>';
	$page .= '<th><b>'.t('Plaats').'</b></th>';
	$page .= '<th><b>'.t('Geplande datum').'</b></th>';
	$page .= '<th><b>'.t('Afgelegde datum').'</b></th>';
	$page .= '</tr>';
	$page .= '</thead>';

	// Show all found address
	$page .= '<tbody>';
	$page_tmp='';
	while ($data = db_fetch_object($queryResult))
	{
		if ((++$i%2)==0) {
			$page_tmp .= '<tr class="even">';
		} else {
			$page_tmp .= '<tr class="odd">';
	 	}
		$page_tmp .= '<td>'.$i.'</td>';	  
		$page_tmp .= '<td>'.oostpoort_hiddenlink('OostpoortForm', 'BzkId', $data->BzkId, 'Edit', $data->Straatnaam).'</td>';
		$page_tmp .= '<td>'.$data->postcode.'</td>';
		$page_tmp .= '<td>'.$data->plaats.'</td>';	
		$page_tmp .= '<td>'.oostpoort_datum($data->geplande).'</td>';
		$page_tmp .= '<td>'.oostpoort_datum($data->afgelegd).'</td>';
		$page_tmp .= '</tr>';
   }

	if ( $page_tmp!='') {
		$page .= $page_tmp;
	} else  {	
		$page .= '<tr class="even">';
		$page .= '<td>'.t('Geen informatie gevonden').'</td>';
		$page .= '</td><td>';
		$page .= '</td><td>';
		$page .= '</td><td>';
		$page .= '</td><td>';
		$page .= '</tr>';
	}
	$page .= '</tbody>';
	$page .= '</table>';

  	// Button bar
 	$page .= oostpoort_hiddenlink('OostpoortForm','','','Search',t('Bezoeken toevoegen')).' | ';
	$page .= oostpoort_hiddenlink('OostpoortForm','','','MainMenu',t('Terug'));
	
	$page .= '</form>';
	$page .= '</div>';

	return $page;  
}

function oostpoort_visit_address_view() {
  
	// Create Breadcrumb menu
	$page  = '<div class="breadcrumb">';
	$page .= '<a href="'.url("").'">Start</a> > ';
	$page .= '<a href="'.url(OOSTPOORT_MENU).'">Oostpoort</a> > ';
	$page .= 'Visit > Address';
	$page .= '</div>';

   // Get posted parameters
	$BzkId=$_POST["BzkId"];
	echo "[BzkId=".$BzkId."]";
		
	$page .= '<div class="oostpoort">';
	
	drupal_set_title(t('Oostpoort bezoekwerk' ) );
		
	$query  = 'select ';
	$query .= ' a.Bzk_Id as BzkId, ';
	$query .= ' concat(b.Adr_Straat, " " ,b.Adr_Huisnr) as straat, ';
	$query .= ' b.Adr_Plaats as AdrPlaats, ';
	$query .= ' b.Adr_Postcode as AdrPostcode, ';	
	$query .= ' b.Adr_Telefoon as AdrTelefoon, ';	
	$query .= ' b.Adr_Id as AdrId, ';
	$query .= ' a.Bzk_Adr_Id as BzkAdrId, ';
	$query .= ' a.Bzk_GeplandDatum as BzkGeplandDatum, ';
	$query .= ' concat(IF(c.Prs_Roepnaam="",c.Prs_Voornamen, c.Prs_Roepnaam)," ", c.Prs_Tussenvoegsels, " ", c.Prs_Achternaam) as Naam ';
	$query .= 'from ';
	$query .= ' oostpoort_bezoek a, oostpoort_adres b, oostpoort_persoon c ';
	$query .= 'where ';
	$query .= ' a.Bzk_Mdw_ID=c.Prs_id and ';
	$query .= ' a.Bzk_Adr_Id = b.Adr_Id and ';
	$query .= ' a.Bzk_Id='.$BzkId;
  
	//echo $query;  
	$queryResult = db_query($query); 
	$data = db_fetch_object($queryResult);
		
	$page .= '<fieldset>' ;
	$page .= '<legend>Adres gegevens</legend>';
	$page .= '<h4>'.$data->Naam.'</h4>';	
	$page .= '<h4>'.$data->straat.'</h4>';	
	$page .= '<h4>'.$data->AdrPlaats.' '.$data->AdrPostcode.'</h4>';		
	$page .= '<h4>'.$data->AdrTelefoon.'</h4>';	
	$page .= '</fieldset>' ;
	
	$page .= oostpoort_visit_address_part($data, false);
		
	// Find all other closed visit entries.
	$query  = 'select ';
	$query .= ' a.Bzk_Id as BzkId, ';
	$query .= ' a.Bzk_BezoekInfo as BzkInfo, ';
	$query .= ' a.Bzk_GeplandDatum as BzkGeplandDatum, ';
	$query .= ' a.Bzk_AfgelegdDatum as BzkAfgelegdDatum, ';
	$query .= ' a.Bzk_GeannuleerdDatum as BzkGeannuleerdDatum, ';
   $query .= ' a.Bzk_BezoekNietGewenstDatum as BzkBezoekNietGewenstDatum, ';
	$query .= ' a.Bzk_Bzt_ID as BzkBztId, ';
	$query .= ' a.Bzk_Adr_Id as BzkAdrId, ';
	$query .= ' concat(IF(b.Prs_Roepnaam="",b.Prs_Voornamen, b.Prs_Roepnaam)," ", b.Prs_Tussenvoegsels, " ", b.Prs_Achternaam) as Naam ';
	$query .= 'from ';
	$query .= '  oostpoort_bezoek a, oostpoort_persoon b ';
	$query .= 'where ';
	$query .= ' a.Bzk_Mdw_ID=b.Prs_id and ';
	$query .= ' a.Bzk_Adr_Id='.$data->AdrId.' and ';
	$query .= ' (a.Bzk_AfgelegdDatum!=0 or ';
	$query .= ' a.Bzk_GeannuleerdDatum!=0 or ';
   $query .= ' a.Bzk_BezoekNietGewenstDatum!=0) '; 
	$query .= 'order by ';
	$query .= ' a.Bzk_Id desc ';
    
   //echo $query;  
	$queryResult = db_query($query); 
	while ($data = db_fetch_object($queryResult))
	{
		$page .= oostpoort_visit_address_part($data, true);
	}
	
   $page .= '</div>';

   return $page;  
}

function oostpoort_visit_address_part($data, $readonly)
{
	$page .= '<form name="OostpoortForm" method="POST" >';
	
	$page .= '<fieldset>';
	if ($data->BzkAfgelegdDatum!=0) {

		$page .= '<legend>Bezoek afgelegd op '.oostpoort_view_date($data->BzkGeplandDatum).' door '.$data->Naam.'</legend>';
		$type = 1;
	
	} else if ($data->BzkBezoekNietGewenstDatum!=0) {
	
		$page .= '<legend>Bezoekt niet gewenst op '.oostpoort_view_date($data->BzkBezoekNietGewenstDatum).' door '.$data->Naam.'</legend>';
		$type = 2;
	
	} else if ($data->BzkGeannuleerdDatum!=0) {
	
		$page .= '<legend>Bezoek geannuleerd op '.oostpoort_view_date($data->BzkGeannuleerdDatum).' door '.$data->Naam.'</legend>';
		$type = 3;
	
	} else {
   	$page .= '<legend>Geboekt op '.oostpoort_view_date($data->BzkGeplandDatum).' door '.$data->Naam.'</legend>';
		$type = 4;
	}
	
	$page .= '<div class="form-item">';
	$page .= '<label>'.t('Bezoek Id').': </label>';
	$page .= $data->BzkId;
	$page .= '</div>';
	
	
	if (($type==2) or ($type==4)) {
		$page .= '<div class="form-item">';
		$page .= '<label>'.t('Bezoek niet gewenst').': </label>';
		$page .= oostpoort_view_checkbox2("NoVisit","ONWAAR",$readonly);
		$page .= '<div class="description">';
		if ($type==4) {
			$page .= t('Als bezoek niet meer gewenst is selecteer deze optie. Gelieve reden op te nemen in notitie veld!. Er zullen dan geen bezoeken meer worden ingepland.');
		}
		$page .= '</div>';
		$page .= '</div>';
	}
	
	if (($type==1) or ($type==3) or ($type==4)) {
		$page .= '<div class="form-item">';
		$page .= '<label>'.t('Bezoek type').': <span class="form-required">*</span></label>';
		$page .= oostpoort_view_type("VisitType", $data->BzkBztId, $readonly);
		$page .= '<div class="description">';
		if ($type==4)  {
			$page .= t('Selecteer wat voor type bezoek is uitgevoerd.');
		}
		$page .= '</div>';
		$page .= '</div>';
	}

	$page .= '<div class="form-item">';
	$page .= '<label>'.t('Bezoek notities').': <span class="form-required">*</span></label>';
	$page .= oostpoort_view_note("BzkInfo", $data->BzkInfo, $readonly);
	$page .= '<div class="description">';
	if ($type==4) {
		$page .= t('Hier kunt u algemene niet vertrouwelijke informatie over het bezoek vastleggen.');
	}
	$page .= '</div>';
	$page .= '</div>';
	
	if (($type==1) or ($type==4)) {
	
		// Get Address members.
		$query  = 'select ';
		$query .= '  a.Prs_Id as PrsId, ';
		$query .= '  concat(IF(a.Prs_Roepnaam="",a.Prs_Voornamen, a.Prs_Roepnaam)," ", a.Prs_Tussenvoegsels, " ", a.Prs_Achternaam) as Naam, ';
		$query .= '  a.Prs_GeslachtMV as PrsGeslacht, '; 
		$query .= '  a.Prs_Geboortedatum as PrsGeboorteDatum, ';
		$query .= '  a.Prs_Huwelijksdatum as PrsHuwelijksdatum, ';
		$query .= '  a.Prs_TelefoonMobiel as PrsMobiel, ';
		$query .= '  a.Prs_EmailAdres as PrsEmail ';  
		$query .= 'from ';
		$query .= '  oostpoort_persoon a ';
		$query .= 'where ';
		$query .= '  a.Prs_Huidig_Adr_ID='.$data->BzkAdrId.' ';
		$query .= 'order by ';
		$query .= '  PrsGeboorteDatum;';
  
		$queryResult = db_query($query);
		//echo $query;
		//echo '<br/>';
	   //echo '<br/>';
  
		// Show Banner
		$page .= '<table>';
		$page .= '<thead>';
		$page .= '<tr>';
		$page .= '<th><b>'.t('Aanwezig').'</b></th>';
		$page .= '<th><b>'.t('Naam').'</b></th>';
		$page .= '<th><b>'.t('Geslacht').'</b></th>';
		$page .= '<th><b>'.t('Geboren').'</b></th>';
		$page .= '<th><b>'.t('Huwelijk').'</b></th>';
		$page .= '<th><b>'.t('Mobiel').'</b></th>';
		$page .= '<th><b>'.t('Email').'</b></th>';	
		$page .= '</tr>';
		$page .= '</thead>';

		// Show all found members
		$page .= '<tbody>';
		$page_tmp='';
		while ($persoon = db_fetch_object($queryResult))
		{
			if ((++$i%2)==0) {
				$page .= '<tr class="even">';
			} else {
				$page .= '<tr class="odd">';
			}
			$page .= '<td>'.oostpoort_view_checkbox2( "Active".$persoon->PrsId, "ONWAAR", $readonly).'</td>';
			$page .= '<td>'.$persoon->Naam.'</td>';
			$page .= '<td>'.oostpoort_visit_admin_geslacht($persoon->PrsGeslacht).'</td>';
			$page .= '<td>'.oostpoort_view_date($persoon->PrsGeboorteDatum).'</td>';
			$page .= '<td>'.oostpoort_view_date($persoon->PrsHuwelijksdatum).'</td>';
			$page .= '<td>'.$persoon->PrsMobiel.'</td>';
			$page .= '<td>'.$persoon->PrsEmail.'</td>';
			$page .= '</tr>';
		}
		$page .= '</tbody>';
		$page .= '</table>';

		// Button bar	
		if ($type==4)
		{
			$page .= oostpoort_hiddenlink('OostpoortForm','BzkId',$data->BzkId,'Ready',t('Bezoek afronden')).' | ';
			$page .= oostpoort_hiddenlink('OostpoortForm','BzkId',$data->BzkId,'DeSelect',t('Bezoek annuleren')).' | ';
			$page .= oostpoort_hiddenlink('OostpoortForm','',0,'Cancel',t('Terug'));	
		}
	}
		
	$page .= '</fieldset>' ;
	$page .= '</form>';
	
	return $page;
}

function oostpoort_visit_list_search() {
  
	// Create Breadcrumb menu
	$page  = '<div class="breadcrumb">';
	$page .= '<a href="'.url("").'">Start</a> > ';
	$page .= '<a href="'.url(OOSTPOORT_MENU).'">Oostpoort</a> > ';
	$page .= 'Visit > Search';
	$page .= '</div>';
	
	$PrsId=$_SESSION["PrsId"];
	echo "[PrsId=".$PrsId."]";
 	
	// Build page
	$page .= '<div class="oostpoort">';
	
	drupal_set_title(t('Oostpoort bezoekwerk' ) );

	$page .= '<form name="OostpoortForm" method="POST" >';
	$page .= oostpoort_view_block(0, false, false);
	$page .= oostpoort_hiddenlink('OostpoortForm','',0,'Filteren',t('Filteren'));
  
	$page .= '<br/>';
	$page .= '<br/>';
   $page .= t('Selecteer nieuwe adressen om te bezoeken.');

	// Query Database
	$query  = 'SELECT ';
	$query .= '  a.Adr_Id as AdrId, ';
	$query .= '  concat (a.Adr_Straat," ", a.Adr_Huisnr) as Straatnaam, ';
	$query .= '  a.Adr_Plaats as AdrPlaats, ';
	$query .= '  a.Adr_postcode as AdrPostcode, ';
	$query .= '  a.Adr_Telefoon as AdrTelefoon ';
	$query .= 'FROM ';
	$query .= '  oostpoort_adres a ';
	$query .= 'where '; 
	$query .= '  a.Adr_GeplandDatum=0 and ';
	$qeury .= '  a.Adr_DeactiveerDatum=0 and ';
	$query .= '  a.Adr_Straat like "b%"';
  
	$queryResult = db_query($query);
	//echo $query;
  
	// Show Banner
	$page .= '<table>';
	$page .= '<thead>';
	$page .= '<tr>';
	$page .= '<th><b>'.t('').'</b></th>';
	$page .= '<th><b>'.t('Straat').'</b></th>';
	$page .= '<th><b>'.t('Postcode').'</b></th>';
	$page .= '<th><b>'.t('Plaats').'</b></th>';
	$page .= '<th><b>'.t('Telefoon').'</b></th>';
	$page .= '<th><b>'.t('Medewerker').'</b></th>';
	$page .= '<th><b>'.t('Laatste Bezoek').'</b></th>';
	$page .= '</tr>';
	$page .= '</thead>';

	// Show all found address
	$page .= '<tbody>';
	$page_tmp='';
	while ($data = db_fetch_object($queryResult))
	{
		if ((++$i%2)==0) {
			$page_tmp .= '<tr class="even">';
		} else {
			$page_tmp .= '<tr class="odd">';
	 	}
		$page_tmp .= '<td>'.oostpoort_view_checkbox2( "Active".$data->AdrId, "ONWAAR", false).'</td>';	  
		$page_tmp .= '<td>'.$data->Straatnaam.'</td>';
		$page_tmp .= '<td>'.$data->AdrPostcode.'</td>';
		$page_tmp .= '<td>'.$data->AdrPlaats.'</td>';	
		$page_tmp .= '<td>'.$data->AdrTelefoon.'</td>';	
		$page_tmp .= '<td>'.$data->Naam.'</td>';
		$page_tmp .= '<td>'.oostpoort_datum($data->afgelegd).'</td>';
		$page_tmp .= '</tr>';
   }

	if ( $page_tmp!='') {
		$page .= $page_tmp;
	} else  {	
		$page .= '<tr class="even">';
		$page .= '<td>'.t('Geen informatie gevonden').'</td>';
		$page .= '</td><td>';
		$page .= '</td><td>';
		$page .= '</td><td>';
		$page .= '</td><td>';
		$page .= '</tr>';
	}
	$page .= '</tbody>';
	$page .= '</table>';

  	// Button bar
 	$page .= oostpoort_hiddenlink('OostpoortForm','',0,'Select',t('Opslaan')).' | ';
	$page .= oostpoort_hiddenlink('OostpoortForm','','','Cancel',t('Terug'));
	
	$page .= '</form>';
	$page .= '</div>';

	return $page;  
}

// ##################################################################################
// ACTIONS
// ##################################################################################

function oostpoort_visit_select()
{
	// Get PrsId from session scope
   $PrsId=$_SESSION["PrsId"];
	echo "[PrsId=".$PrsId."]";
	
	// Query Database
	$query  = 'SELECT ';
	$query .= '  a.Adr_Id as AdrId, ';
	$query .= '  concat (a.Adr_Straat," ", a.Adr_Huisnr) as Straatnaam, ';
	$query .= '  a.Adr_Plaats as AdrPlaats, ';
	$query .= '  a.Adr_postcode as AdrPostcode, ';
	$query .= '  a.Adr_Telefoon as AdrTelefoon ';
	$query .= 'FROM ';
	$query .= '  oostpoort_adres a ';
	$query .= 'where '; 
	$query .= '  a.Adr_GeplandDatum=0 and ';
	$qeury .= '  a.Adr_DeactiveerDatum=0 and ';
	$query .= '  a.Adr_Straat like "b%"';
			 
	$queryResult = db_query($query);
	//echo $query;
	
	while ($data = db_fetch_object($queryResult))
	{
	   $i++;
		$value=$_POST["Active".$data->AdrId];		
		if ($value=="on") {
			
			$amount++;

			// Check if entry all exist (Protect against reload button), if so skip insert)
			$query  = 'select Adr_GeplandDatum as AdrGeplandDatum from oostpoort_adres where Adr_id='.$data->AdrId;
			$queryResult = db_query($query);
			$tmp = db_fetch_object($queryResult);
			if ($tmp->AdrGeplandDatum==0)
			{			
			   // Insert new visit
				$query = 'INSERT INTO oostpoort_bezoek (';
				$query .= '  `Bzk_Mdw_ID`, `Bzk_Adr_ID`, `Bzk_GeplandDatum`, `Bzk_Bzt_ID`)';
				$query .= 'VALUES ';
				$query .= '( '.$PrsId.','.$data->AdrId.', "'.date("Y-m-d").'", 1 )';
				
				echo $query;
				db_query($query);
					
				// Update adres entry
				$query = 'UPDATE oostpoort_adres SET Adr_GeplandDatum="'.date("Y-m-d").'" where Adr_id='.$data->AdrId;
				
				echo $query;
				db_query($query);
			}
		}
	}

   if ($amount>0) {	
		$page .= '<div class="messages status">';
		$page .= t($amount.' bezoeken zijn ingepland.');
		$page .= '</div>';
	}
	
	return $page;
}


function oostpoort_visit_deselect()
{	
   // Get posted parameters
	$BzkId=$_POST["BzkId"];
	echo "[BzkId=".$BzkId."]";
	
	$BzkInfo=$_POST["BzkInfo"];
	echo "[BzkInfo=".$BzkInfo."]";	
	
	// Update database
	$query  = 'UPDATE oostpoort_bezoek SET ';
	$query .= '  Bzk_GeannuleerdDatum="'.date("Y-m-d").'", ';
	$query .= '  Bzk_BezoekInfo="'.$BzkInfo.'" ';
	$query .= 'where ';
	$query .= '  Bzk_ID='.$BzkId;
	echo $query;
	db_query($query); 
	
	$query  = 'select Bzk_Adr_Id as BzkAdrId  from oostpoort_bezoek where Bzk_id='.$BzkId;
	$queryResult = db_query($query);
	echo $query;
	$data = db_fetch_object($queryResult);

	// Update adres entry
	$query = 'UPDATE oostpoort_adres SET Adr_GeplandDatum="0" where Adr_id='.$data->BzkAdrId;
	echo $query;
	db_query($query);
			
	$page .= '<div class="messages status">';
   $page .= t('Bezoek '.$BzkId.' is geannuleerd');
	$page .= '</div>';
	
	return $page;
}

function oostpoort_visit_ready()
{
   // Get posted parameters
	$BzkId=$_POST["BzkId"];
	echo "[BzkId=".$BzkId."]";
	
	$BzkInfo=$_POST["BzkInfo"];
	echo "[BzkInfo=".$BzkInfo."]";	
	
	$NoVisit=$_POST["NoVisit"];
	echo "[NoVisit=".$NoVisit."]";

   $VisitType=$_POST["VisitType"];
	echo "[VisitType=".$VisitType."]";
	
	// Update database
	$query  = 'UPDATE oostpoort_bezoek SET ';
	if (NoVisit=="on") {
	   $query .= '  Bzk_BezoekNietGewenstDatum="'.date("Y-m-d").'", ';	
	} else {
		$query .= '  Bzk_AfgelegdDatum="'.date("Y-m-d").'", ';
	}
	$query .= '  Bzk_Bzt_ID="'.$VisitType.'", ';
	$query .= '  Bzk_BezoekInfo="'.$BzkInfo.'" ';
	$query .= 'where ';
	$query .= '  Bzk_ID='.$BzkId;
	
	echo $query;
	db_query($query); 

	$query  = 'select Bzk_Adr_Id as BzkAdrId  from oostpoort_bezoek where Bzk_id='.$BzkId;
	$queryResult = db_query($query);
	echo $query;
	$data = db_fetch_object($queryResult);

	// Update adres entry
	$query = 'UPDATE oostpoort_adres SET Adr_GeplandDatum="0" where Adr_id='.$data->BzkAdrId;
	echo $query;
	db_query($query);
				
	$page .= '<div class="messages status">';
   $page .= t('Bezoek '.$BzkId.' is afgerond');
	$page .= '</div>';
	
	return $page;
}

// ##################################################################################
// UTILS
// ##################################################################################

function oostpoort_view_note($id, $value, $readonly) {

   if ( ! $readonly ) {
		return '<textarea name="'.$id.'" cols=85 rows=4>'.$value.'</textarea>';
	} else {
	   return $value;
	}
}

function oostpoort_view_type($id, $value, $readonly) {

   if ( ! $readonly ) {
		$query  = 'select Bzt_ID, Bzt_Omschrijving FROM {oostpoort_bezoektype} order by Bzt_ID';
		$queryResult = db_query($query);     
		
		$page.='<select name="VisitType">';
		while ($data = db_fetch_object($queryResult)) {
		
			$page.='<option value="'.$data->Bzt_ID.'" ';
			if ($data->Bzt_ID==value) $page.='selected="selected" ';
			$page.='>'.$data->Bzt_Omschrijving.'</option>';
		}
		$page.='</select> ';
	  
   } else {
	 
      $query  = 'SELECT Bzt_Omschrijving FROM {oostpoort_bezoektype} WHERE Bzt_ID='.$value;
      $queryResult = db_query($query);
      $data = db_fetch_object($queryResult);
		 
      $page .= $data->Bzt_Omschrijving;
   }
   return $page;
}

function oostpoort_view_checkbox2( $id, $value, $readonly ) {

	if ( ! $readonly ) {
		if ($value=="WAAR") {
			$page = '<input type="checkbox" name="'.$id.'" checked="true" />';
		} else {
			$page = '<input type="checkbox" name="'.$id.'" />';
		}
	} else {
		if ($value=="WAAR") {
			$page="";
		}
	}
	return $page;
}


// ##################################################################################
// THE END
// ##################################################################################