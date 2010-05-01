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
	$Action=$_POST['Action'];
	if ($Action=="") {
		$Action=$_SESSION['Action'];
	}
	oostpoort_debug_param("Action", $Action);
	$_SESSION['Action']=$Action;
    
	if ($Action=="Edit") {
	
		$page .= oostpoort_visit_address_view();
	
	} else if ($Action=="Cancel") {
		
		$page .= oostpoort_visit_list_view();
	
	} else if ($Action=="Search") {

		$page .= oostpoort_visit_list_search();
	
	} else if ($Action=="Select") {

		$page .= oostpoort_visit_select();
		$page .= oostpoort_visit_list_view();
	
	} else if ($Action=="DeSelect") {
	
		$message = oostpoort_visit_deselect();
		if ($message["error"]==0) {
		
			$page .= $message["page"];
			$page .= oostpoort_visit_list_view();

		} else {
		
			$page .= $message["page"];
			$page .= oostpoort_visit_address_view();
			
		}
	
	} else if ($Action=="Ready") {
	
		$message = oostpoort_visit_ready();
		if ($message["error"]==0)
		{
			$page .= $message["page"];
			$page .= oostpoort_visit_list_view();
			
		} else {
			$page .= $message["page"];
			$page .= oostpoort_visit_address_view();
		}
	
	} else {

		$page = oostpoort_visit_list_view();
	}

   if ($page!="") {
		print theme("page", $page);
	}
}

// ##################################################################################
// VIEWS
// ##################################################################################

function oostpoort_visit_list_view() {
  
	// Create Breadcrumb menu
	$page  = '<div class="breadcrumb">';
	$page .= '<a href="'.url("").'">Start</a> > ';
	$page .= '<a href="'.url(OOSTPOORT_MENU).'">Oostpoort</a> > ';
	$page .= t('Bezoekwerk');
	$page .= '</div>';

   // Get posted parameters
	$PrsId=$_POST["PrsId"];
	if ($PrsId=="") {
	   $PrsId=$_SESSION["PrsId"];
		if ($PrsId=="") {
			$PrsId=0;
		}
	}	
	oostpoort_debug_param("PrsId", $PrsId);
	
	// Set Session scope variables
	$_SESSION["PrsId"]=$PrsId;
	
	// Build page
	$page .= '<div class="oostpoort">';
	
	drupal_set_title(t('Bezoekwerk' ) );

	$page .= '<form name="OostpoortForm" method="POST" >';
  
	$page .= '<fieldset>' ;
	$page .= '<legend>'.t('Filteren').'</legend>';
	$page .= oostpoort_view_visiters("PrsId", $PrsId);
	$page .= '</fieldset>' ;
	
 	// Query Database
	$query  = 'select ';
	$query .= '   (select concat(Prs_Tussenvoegsels, " ", substring_index(Prs_Achternaam,"-",1)) from oostpoort_persoon where Prs_Huidig_Adr_Id=b.Adr_id limit 1) as Naam, ';
	$query .= '   concat (b.Adr_Straat," ", b.Adr_Huisnr) as Straatnaam, ';
	$query .= '   b.Adr_Plaats as AdrPlaats, ';
	$query .= '   b.Adr_postcode as AdrPostcode, ';
	$query .= '   a.Bzk_GeplandDatum as BzkGeplande, ';
	$query .= '   a.Bzk_AfgelegdDatum as BzkAfgelegd, ';
	$query .= '   a.Bzk_ID as BzkId '; 
	$query .= 'from ';
	$query .= '   oostpoort_bezoek a, oostpoort_adres b ';
	$query .= 'where ';
	$query .= '   a.Bzk_Adr_ID=b.Adr_id and ';
	$query .= '   a.Bzk_AfgelegdDatum=0 and ';
	$query .= '   a.Bzk_GeannuleerdDatum=0 and ';
   $query .= '   a.Bzk_BezoekNietGewenstDatum=0 and '; 
	$query .= '   a.Bzk_Mdw_id='.$PrsId.' ';
	$query .= 'order by ';
	$query .= '   Straatnaam';
   oostpoort_debug_sql($query);
	$queryResult = db_query($query);
  
	// Show Banner
	$page .= '<table>';
	$page .= '<tr>';
	$page .= '<thead>';
	$page .= '<tr>';
	//$page .= '<th><b>'.t('Rij').'</b></th>';
	$page .= '<th><b>'.t('Naam').'</b></th>';
	$page .= '<th class="active"><b>'.t('Straat').'</b></th>';
	$page .= '<th><b>'.t('Postcode').'</b></th>';
	$page .= '<th><b>'.t('Plaats').'</b></th>';
	$page .= '<th><b>'.t('Geplande').'</b></th>';
	$page .= '<th><b>'.t('Afgelegde').'</b></th>';
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
		//$page_tmp .= '<td>'.$i.'</td>';	  
		$page_tmp .= '<td>'.$data->Naam.'</td>';	  
		$page_tmp .= '<td class="active">'.oostpoort_hiddenlink('OostpoortForm', 'BzkId', $data->BzkId, 'Edit', $data->Straatnaam).'</td>';
		$page_tmp .= '<td>'.$data->AdrPostcode.'</td>';
		$page_tmp .= '<td>'.$data->AdrPlaats.'</td>';	
		$page_tmp .= '<td>'.oostpoort_view_date($data->BzkGeplande).'</td>';
		$page_tmp .= '<td>'.oostpoort_view_date($data->BzkAfgelegd).'</td>';
		$page_tmp .= '</tr>';
   }

	if ( $page_tmp!='') {
		$page .= $page_tmp;
	} else  {	
		$page .= '<tr class="even">';
		$page .= '<td>'.t('Geen informatie gevonden').'</td>';
		$page .= '</tr>';
	}
	$page .= '</tbody>';
	$page .= '</table>';
	
	$page .= oostpoort_hiddenlink('OostpoortForm','','','Search',t('Bezoeken toevoegen')).' | ';
	$page .= '<a href="'.url(OOSTPOORT_MENU).'">'.t('Terug').'</a>';
	
	$page .= '</form>';
	$page .= '</div>';

	return $page;  
}

function oostpoort_visit_address_view() {
  
	// Create Breadcrumb menu
	$page  = '<div class="breadcrumb">';
	$page .= '<a href="'.url("").'">Start</a> > ';
	$page .= '<a href="'.url(OOSTPOORT_MENU).'">Oostpoort</a> > ';
	$page .= 'Bezoekwerk > Bezoek Informatie';
	$page .= '</div>';

   // Get posted parameters
	$BzkId=$_POST["BzkId"];
	oostpoort_debug_param("BzkId", $BzkId);

	$page .= '<form name="OostpoortForm" method="POST" >';
		
	$page .= '<div class="oostpoort">';
	
	drupal_set_title(t('Bezoek Informatie' ) );
		
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
	oostpoort_debug_sql($query);
	$queryResult = db_query($query); 
	$data = db_fetch_object($queryResult);
		
	$page .= '<fieldset>' ;
	$page .= '<legend>Adres gegevens</legend>';
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
   oostpoort_debug_sql($query);
	$queryResult = db_query($query); 
	while ($data = db_fetch_object($queryResult))
	{
		$page .= oostpoort_visit_address_part($data, true);
	}
	
	$page .= oostpoort_hiddenlink('OostpoortForm','',0,'Cancel',t('Terug'));	
		
   $page .= '</div>';
	$page .= '</form>';
	
   return $page;  
}

function oostpoort_visit_address_part($data, $readonly)
{	
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
	
	//$page .= '<div class="form-item">';
	//$page .= '<label>'.t('Bezoek Id').': </label>';
	//$page .= $data->BzkId;
	//$page .= '</div>';
	
	
	if (($type==2) or ($type==4)) {
		$page .= '<div class="form-item">';
		$page .= '<label>'.t('Bezoek niet gewenst').': </label>';
		$page .= oostpoort_view_checkbox2("NoVisit","ONWAAR",$readonly);
		$page .= '<div class="description">';
		if ($type==4) {
			$page .= t('Als bezoek niet meer gewenst is selecteer deze optie. Er zullen dan geen bezoeken meer worden ingepland.');
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
	
		if (!$readonly) {
			
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
			
		} else {
		
			$query  = 'select ';
			$query .= '  a.Prs_Id as PrsId, ';
			$query .= '  concat(IF(a.Prs_Roepnaam="",a.Prs_Voornamen, a.Prs_Roepnaam)," ", a.Prs_Tussenvoegsels, " ", a.Prs_Achternaam) as Naam, ';
			$query .= '  a.Prs_GeslachtMV as PrsGeslacht, '; 
			$query .= '  a.Prs_Geboortedatum as PrsGeboorteDatum, ';
			$query .= '  a.Prs_Huwelijksdatum as PrsHuwelijksdatum, ';
			$query .= '  a.Prs_TelefoonMobiel as PrsMobiel, ';
			$query .= '  a.Prs_EmailAdres as PrsEmail ';  
			$query .= 'from ';
			$query .= '  oostpoort_persoon a, oostpoort_bezoekpersoon b ';
			$query .= 'where ';
			$query .= '  b.Bzp_Bzk_ID='.$data->BzkId.' and ';
			$query .= '  a.Prs_Id = b.Bzp_Prs_ID ';
			$query .= 'order by ';
			$query .= '  PrsGeboorteDatum;';
		}
		oostpoort_debug_sql($query);
		$queryResult = db_query($query);
  
		// Show Banner
		$page .= '<table>';
		$page .= '<thead>';
		$page .= '<tr>';
		$page .= '<th><b>'.t('Aanwezig').'<span class="form-required">*</span></b></th>';
		$page .= '<th><b>'.t('Naam').'</b></th>';
		$page .= '<th><b>'.t('Geslacht').'</b></th>';
		$page .= '<th class="active"><b>'.t('Geboren').'</b></th>';
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
			$page .= '<td>'.oostpoort_view_geslacht($persoon->PrsGeslacht).'</td>';
			$page .= '<td class="active">'.oostpoort_view_date($persoon->PrsGeboorteDatum).'</td>';
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
			$page .= oostpoort_hiddenlink('OostpoortForm','BzkId',$data->BzkId,'DeSelect',t('Bezoek annuleren'));
		}
	}
		
	$page .= '</fieldset>' ;
	
	return $page;
}

function oostpoort_visit_list_search() {
  
	// Create Breadcrumb menu
	$page  = '<div class="breadcrumb">';
	$page .= '<a href="'.url("").'">Start</a> > ';
	$page .= '<a href="'.url(OOSTPOORT_MENU).'">Oostpoort</a> > ';
	$page .= 'Bezoekwerk > Bezoeken plannen';
	$page .= '</div>';
	
	$PrsId=$_SESSION["PrsId"];
	oostpoort_debug_param("PrsId", $PrsId);
 	
	// Get search value out post or session scope
	$Search=$_POST['Search'];
	oostpoort_debug_param("Search", $Search);
	
	// Get Sort value 
	$Sort=$_POST['Sort'];
	if ($Sort=="") {
		$Sort=$_SESSION['Sort'];
		if ($Sort=="") {
			$Sort="A";
		}
	}
	
	$_SESSION['Sort']=$Sort;
	oostpoort_debug_param("Sort", $Sort);
	
	// Build page
	$page .= '<div class="oostpoort">';
	
	drupal_set_title(t('Bezoeken plannen' ) );

	$page .= '<form name="OostpoortForm" method="POST" >';
	
	$page .= '<fieldset>' ;
	$page .= '<legend>Filteren</legend>';
	
	$page .= '<div class="form-item">';
	$page .= '<label>'.t('Straatnaam').' :</label>';
	$page .= '<input id="text" name="Search" size="40" maxlength="40" type="text" value="'.$Search.'" ';
	$page .= '<div class="description">';
	$page .= t('Filteren op straatnaam.');
	$page .= '</div>';
	$page .= '</div>';

	$page .= '<div class="form-item">';
	$page .= '<label>'.t('Blok').' :</label>';
	$page .= oostpoort_view_block(0, false, false);
	$page .= '<div class="description">';
	$page .= t('Filteren op blok.');
	$page .= '</div>';
	$page .= '</div>';
	
	$page .= oostpoort_sortbar($Sort);
	
   $page .= '</fieldset>' ;
  
   $page .= t('Selecteer nieuwe adressen om te bezoeken.');

	// Query Database
	$query  = 'SELECT ';
	$query .= '  distinct a.Adr_id as AdrId, ';
	$query .= '  a.Adr_GeplandDatum as AdrGeplandDatum, ';
	$query .= '  a.Adr_DeactiveerDatum as AdrDeactiveerDatum, ';
	$query .= '  concat (a.Adr_Straat," ", a.Adr_Huisnr) as Straatnaam, ';
	$query .= '  a.Adr_Plaats as AdrPlaats, ';
	$query .= '  a.Adr_postcode as AdrPostcode, ';
	$query .= '  a.Adr_Telefoon as AdrTelefoon, '; 
	$query .= '  (select concat(Prs_Tussenvoegsels, " ", substring_index(Prs_Achternaam,"-",1)) from oostpoort_persoon where Prs_Huidig_Adr_Id=a.Adr_id limit 1) as Naam ';
	$query .= 'FROM ';
	$query .= '  oostpoort_adres a  ';
	$query .= 'where '; 
	if (strlen($Search)==0) {
		$query  .= 'a.Adr_Straat LIKE "'.$Sort.'%" ';
	} else { 
		$query  .= 'a.Adr_Straat LIKE "'.$Search.'%" ';
   }
	$query .= 'order by ';
	$query .= '  Straatnaam'; 
  
   oostpoort_debug_sql($query);
	$queryResult = db_query($query);
  
	// Show Banner
	$page .= '<table>';
	$page .= '<thead>';
	$page .= '<tr>';
	$page .= '<th><b>'.t('').'</b></th>';
	$page .= '<th><b>'.t('Naam').'</b></th>';
	$page .= '<th class="active"><b>'.t('Straat').'</b></th>';
	//$page .= '<th><b>'.t('Postcode').'</b></th>';
	$page .= '<th><b>'.t('Plaats').'</b></th>';
	//$page .= '<th><b>'.t('Telefoon').'</b></th>';
	$page .= '<th><b>'.t('Gepland').'</b></th>';
	$page .= '<th><b>'.t('Deactief').'</b></th>';
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
		if (($data->AdrGeplandDatum==0) and ($data->AdrDeactiveerDatum==0)) {
			$page_tmp .= '<td>'.oostpoort_view_checkbox2( "Active".$data->AdrId, "ONWAAR", false).'</td>';	  
		} else {
			$page_tmp .= '<td></td>';
		}
		$page_tmp .= '<td>'.$data->Naam."</td>";
		$page_tmp .= '<td class="active">'.$data->Straatnaam.'</td>';
		//$page_tmp .= '<td>'.$data->AdrPostcode.'</td>';
		$page_tmp .= '<td>'.$data->AdrPlaats.'</td>';	
		//$page_tmp .= '<td>'.$data->AdrTelefoon.'</td>';	
		$page_tmp .= '<td>'.oostpoort_view_date($data->AdrGeplandDatum).'</td>';
		$page_tmp .= '<td>'.oostpoort_view_date($data->AdrDeactiveerDatum).'</td>';
		$page_tmp .= '</tr>';
   }

	if ( $page_tmp!='') {
		$page .= $page_tmp;
	} else  {	
		$page .= '<tr class="even">';
		$page .= '<td>'.t('Geen informatie gevonden').'</td>';
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
	oostpoort_debug_param("PrsId", $PrsId);
	
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
	$query .= '  a.Adr_Straat like "b%" ';
	$query .= 'order by ';
	$query .= '  Straatnaam '; 
			 
	oostpoort_debug_sql($query);
	$queryResult = db_query($query);	
	while ($data = db_fetch_object($queryResult))
	{
	   $i++;
		$value=$_POST["Active".$data->AdrId];		
		oostpoort_debug_param("Active".$data->AdrId, $value);
		if ($value=="on") {
			
			$amount++;

			// Check if entry all exist (Protect against reload button), if so skip insert)
			$query  = 'select Adr_GeplandDatum as AdrGeplandDatum from oostpoort_adres where Adr_id='.$data->AdrId;
			$queryResultTmp = db_query($query);
			$tmp = db_fetch_object($queryResultTmp);
			if ($tmp->AdrGeplandDatum==0)
			{			
			   // Insert new visit
				$query = 'INSERT INTO oostpoort_bezoek (';
				$query .= '  `Bzk_Mdw_ID`, `Bzk_Adr_ID`, `Bzk_GeplandDatum`, `Bzk_Bzt_ID`)';
				$query .= 'VALUES ';
				$query .= '( '.$PrsId.','.$data->AdrId.', "'.date("Y-m-d").'", 1 )';
				
				oostpoort_debug_sql($query);
				db_query($query);
					
				// Update adres entry
				$query = 'UPDATE oostpoort_adres SET Adr_GeplandDatum="'.date("Y-m-d").'" where Adr_id='.$data->AdrId;
				
				oostpoort_debug_sql($query);
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
	oostpoort_debug_param("BzkId", $BzkId);
	
	$BzkInfo=$_POST["BzkInfo"];
	oostpoort_debug_param("BzkInfo", $BzkInfo);
	
	if ($BzkInfo=="") {
		$page .= '<div class="messages error">';
		$page .= t('Bezoek notitie is verplicht');
		$page .= '</div>';
		
		$message["page"]=$page;
		$message["error"]=1;
		return $message;
	}
	
	// Update database
	$query  = 'UPDATE oostpoort_bezoek SET ';
	$query .= '  Bzk_GeannuleerdDatum="'.date("Y-m-d").'", ';
	$query .= '  Bzk_BezoekInfo="'.$BzkInfo.'" ';
	$query .= 'where ';
	$query .= '  Bzk_ID='.$BzkId;
	
	oostpoort_debug_sql($query);
	db_query($query); 
	
	$query  = 'select Bzk_Adr_Id as BzkAdrId  from oostpoort_bezoek where Bzk_id='.$BzkId;
	oostpoort_debug_sql($query);
	$queryResult = db_query($query);
	$data = db_fetch_object($queryResult);

	// Update adres entry
	$query = 'UPDATE oostpoort_adres SET Adr_GeplandDatum="0" where Adr_id='.$data->BzkAdrId;
	oostpoort_debug_sql($query);
	db_query($query);
			
	$page .= '<div class="messages status">';
   $page .= t('Bezoek '.$BzkId.' is geannuleerd');
	$page .= '</div>';
	
	$message["page"]=$page;
	$message["error"]=0;
	
	return $message;
}

function oostpoort_visit_ready()
{
   // Get posted parameters
	$BzkId=$_POST["BzkId"];
	oostpoort_debug_param("BzkId", $BzkId);
	
	$BzkInfo=$_POST["BzkInfo"];
	oostpoort_debug_param("BzkInfo", $BzkInfo);
	
	if ($BzkInfo=="")
	{
		$page .= '<div class="messages error">';
		$page .= t('Bezoek notitie is verplicht');
		$page .= '</div>';
		
		$message["page"]=$page;
		$message["error"]=1;
		return $message;
	}
		
	$NoVisit=$_POST["NoVisit"];
	oostpoort_debug_param("NoVisit", $NoVisit);

   $VisitType=$_POST["VisitType"];
	oostpoort_debug_param("VisitType", $VisitType);
	
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
	
	oostpoort_debug_sql($query);
	db_query($query); 

	$query  = 'select Bzk_Adr_Id as BzkAdrId  from oostpoort_bezoek where Bzk_id='.$BzkId.'; ';
	oostpoort_debug_sql($query);
	$queryResult = db_query($query);
	$data = db_fetch_object($queryResult);

	// Update adres entry
	$query = 'UPDATE oostpoort_adres SET Adr_GeplandDatum="0" where Adr_id='.$data->BzkAdrId.'; ';
	oostpoort_debug_sql($query);
	db_query($query);

	// Check if entry all exist (Protect against reload button), if so skip insert
	$query  = 'select count(*) as Amount from oostpoort_bezoekpersoon where Bzp_Bzk_ID='.$BzkId.';';
	oostpoort_debug_sql($query);
	$queryResult = db_query($query);
	$tmp = db_fetch_object($queryResult);
	if ($tmp->Amount==0) {			
		$query  = 'select ';
		$query .= '  a.Prs_Id as PrsId ';
		$query .= 'from ';
		$query .= '  oostpoort_persoon a ';
		$query .= 'where ';
		$query .= '  a.Prs_Huidig_Adr_ID='.$data->BzkAdrId.' ';
		$query .= 'order by ';
		$query .= '  a.Prs_GeboorteDatum; ';
					
		oostpoort_debug_sql($query);
		$queryResult = db_query($query);
		while ($data = db_fetch_object($queryResult))
		{
			$value=$_POST["Active".$data->PrsId];		
			oostpoort_debug_param("Active".$data->PrsId, $value);
			
			if ($value=="on") {
		
				// Insert new visit
				$query = 'INSERT INTO oostpoort_bezoekpersoon (';
				$query .= '  `Bzp_Bzk_ID`, `Bzp_Prs_ID`) ';
				$query .= 'VALUES ';
				$query .= '( '.$BzkId.','.$data->PrsId.' ); ';
		
				oostpoort_debug_sql($query);
				db_query($query);		
			}
		}
		$page .= '<div class="messages status">';
		$page .= t('Bezoek '.$BzkId.' is afgerond');
		$page .= '</div>';
   }
	
	$message["page"]=$page;
	$message["error"]=0;
		
	return $message;
}

// ##################################################################################
// THE END
// ##################################################################################