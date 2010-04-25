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
	$action=$_POST['action'];
	echo "[action=".$action."]";
      
	if ($action=="Edit") {
		
		$page = oostpoort_visit_address_view();
		
	} else if ($action=="Cancel") {
		
		$page = oostpoort_visit_list_view();
	
	} else if ($action=="Search") {

		$page = oostpoort_visit_list_search();
	
	} else if ($action=="Select") {

		// Store selected addresses.
		$page = oostpoort_visit_list_view();
	
	} else {

		$page = oostpoort_visit_list_view();
	}
    
	print theme("page", $page);
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
		$PrsId=0;
	}	
	echo "[PrsId=".$PrsId."]";
	
   // Store PrsId on session scope
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
	$page .= '<select name="PrsId">';   
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
	$query .= '  concat (c.Adr_Straat," ", c.Adr_Huisnr) as Straatnaam, ';
	$query .= '  c.Adr_Plaats as plaats, ';
	$query .= '  c.Adr_postcode as postcode, ';
	$query .= '  a.Bzk_GeplandDatum as geplande, ';
	$query .= '  a.Bzk_AfgelegdDatum as afgelegd, ';
	$query .= '  a.Bzk_Adr_ID as AdrId '; 
	$query .= 'from ';
	$query .= '   oostpoort_bezoek a, oostpoort_adres c ';
	$query .= 'where ';
	$query .= '   a.Bzk_Adr_ID=c.Adr_id and ';
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
		$page_tmp .= '<td>'.oostpoort_hiddenlink('OostpoortForm', 'AdrId', $data->AdrId, 'Edit', $data->Straatnaam).'</td>';
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

	/*$query  = 'select distinct concat (d.Adr_Straat, " ", d.adr_Huisnr) as straatnaam, ';
	$query .= 'd.Adr_Plaats as plaats, ';
	$query .= 'd.Adr_Postcode as postcode, ';
	$query .= 'a.bzk_geplandDatum as geplande, '; 
	$query .= 'a.Bzk_AfgelegdDatum as afgelegd, ';
	$query .= 'd.Adr_Id as id '; 
	$query .= 'from oostpoort_bezoek a, oostpoort_bezoekpersoon b, oostpoort_persoon c, oostpoort_adres d '; 
	$query .= 'where a.Bzk_id=b.Bzp_Bzk_id and ';
	$query .= 'b.Bzp_Prs_id=c.Prs_id and ';
	$query .= 'c.Prs_Huidig_Adr_id=d.Adr_id and ';
	$query .= 'a.Bzk_Mdw_id='.$block;
	$query .= ' order by straatnaam';
	$queryResult = db_query($query);

	// Show Banner
	$page .= '<table width=100% border="1" cellpadding="3" cellspacing="3">';
	$page .= '<tr>';
	$page .= '<td><b>'.t('Straat').'</b></td>';
	$page .= '<td><b>'.t('Postcode').'</b></td>';
	$page .= '<td><b>'.t('Plaats').'</b></td>';
	$page .= '<td><b>'.t('Geplande datum').'</b></td>';
	$page .= '<td><b>'.t('Afgelegde datum').'</b></td>';
	$page .= '</tr>';

	// Show all found address
	$page_tmp='';
	while ($data = db_fetch_object($queryResult))
	{
		$page_tmp .= '<tr>';
		$page_tmp .= '<td>'.l($data->straatnaam, OOSTPOORT_VISIT_DETAIL."/".$data->id).'</td>';;
		$page_tmp .= '<td>'.$data->postcode.'</td>';
		$page_tmp .= '<td>'.$data->plaats.'</td>';	
		$page_tmp .= '<td>'.oostpoort_datum($data->geplande).'</td>';
		$page_tmp .= '<td>'.oostpoort_datum($data->afgelegd).'</td>';
		$page_tmp .= '</tr>';
	}

	if ( $page_tmp!='') {
		// Show Content
		$page .= $page_tmp;
	}
	else 
	{
		// No content found
		$page .= '<tr><td>';
		$page .= t('Geen informatie gevonden');
		$page .= '</td><td>';
		$page .= '</td><td>';
		$page .= '</td><td>';
		$page .= '</td></tr>';
	}
	$page .= '</table>';*/
  
  	// Button bar
 	$page .= oostpoort_hiddenlink('OostpoortForm','Id','0','Search',t('Toevoegen'));
	
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
	$AdrId=$_POST["AdrId"];
	echo "[AdrId=".$AdrId."]";
	
	$page .= '<div class="oostpoort">';
	
	drupal_set_title(t('Oostpoort bezoekwerk' ) );
		
	$page .= '<fieldset>' ;
	$page .= '<legend>Adres gegevens</legend>';

	$query  = 'select ';
	$query .= ' concat(a.Adr_Straat, " " ,a.Adr_Huisnr) as straat, ';
	$query .= ' a.Adr_Plaats as AdrPlaats, ';
	$query .= ' a.Adr_Postcode as AdrPostcode, ';	
	$query .= ' a.Adr_Telefoon as AdrTelefoon ';	
	$query .= 'from ';
	$query .= '  oostpoort_adres a ';
	$query .= 'where ';
	$query .= ' a.Adr_id='.$AdrId;
  
	//echo $query;  
	$queryResult = db_query($query); 
	$data = db_fetch_object($queryResult);
	
	$page .= '<h4>'.$data->Naam.'</h4>';	
	$page .= '<h4>'.$data->straat.'</h4>';	
	$page .= '<h4>'.$data->AdrPlaats.' '.$data->AdrPostcode.'</h4>';		
	$page .= '<h4>'.$data->AdrTelefoon.'</h4>';	
	$page .= '</fieldset>' ;
	  
	  
	// Query Database
	$page .= '<form name="OostpoortForm" method="POST" >';
	$page .= '<fieldset>' ;
	$page .= '<legend>Bezoek 12-02-2009</legend>';
	
	$query  = 'select ';
	$query .= '  concat(IF(a.Prs_Roepnaam="",a.Prs_Voornamen, a.Prs_Roepnaam)," ", a.Prs_Tussenvoegsels, " ", a.Prs_Achternaam) as Naam, ';
	$query .= '  a.Prs_GeslachtMV as PrsGeslacht, '; 
	$query .= '  a.Prs_Geboortedatum as PrsGeboorteDatum, ';
   $query .= '  a.Prs_TelefoonMobiel as PrsMobiel, ';
	$query .= '  a.Prs_EmailAdres as PrsEmail ';  
	$query .= 'from ';
	$query .= '  oostpoort_persoon a ';
	$query .= 'where ';
	$query .= '  a.Prs_Huidig_Adr_ID='.$AdrId.' ';
	$query .= 'order by ';
	$query .= '  PrsGeboorteDatum';
  
	$queryResult = db_query($query);
	//echo $query;
  
	// Show Banner
	$page .= '<table>';
	$page .= '<tr>';
	$page .= '<thead>';
	$page .= '<tr>';
	$page .= '<th><b>'.t('Naam').'</b></th>';
	$page .= '<th><b>'.t('Geslacht').'</b></th>';
	$page .= '<th><b>'.t('Geboorte Datum').'</b></th>';
	$page .= '<th><b>'.t('Mobiel').'</b></th>';
	$page .= '<th><b>'.t('Email').'</b></th>';	
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
		$page_tmp .= '<td>'.$data->Naam.'</td>';
		$page_tmp .= '<td>'.oostpoort_visit_admin_geslacht($data->PrsGeslacht).'</td>';
		$page_tmp .= '<td>'.oostpoort_view_date($data->PrsGeboorteDatum).'</td>';
		$page_tmp .= '<td>'.$data->PrsMobiel.'</td>';
		$page_tmp .= '<td>'.$data->PrsEmail.'</td>';
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
	$page .= '</fieldset>' ;
		
  	// Button bar
 	$page .= oostpoort_hiddenlink('OostpoortForm','PrsId',$_SESSION["PrsId"],'Cancel',t('Terug'));
	
   $page .= '</form>';
   $page .= '</div>';

   return $page;  
}

function oostpoort_visit_list_search() {
  
	// Create Breadcrumb menu
	$page  = '<div class="breadcrumb">';
	$page .= '<a href="'.url("").'">Start</a> > ';
	$page .= '<a href="'.url(OOSTPOORT_MENU).'">Oostpoort</a> > ';
	$page .= 'Visit > Search';
	$page .= '</div>';

   // Get posted parameters
	$PrsId=$_POST["PrsId"];
	if ($PrsId=="") {
		$PrsId=0;
	}	
	echo "[PrsId=".$PrsId."]";
	
   // Store PrsId on session scope
   $_SESSION["PrsId"]=$PrsId;
  	
	// Build page
	$page .= '<div class="oostpoort">';
	
	drupal_set_title(t('Oostpoort bezoekwerk' ) );

	$page .= '<form name="OostpoortForm" method="POST" >';
     
	$query  = 'select ';
	$query .= '  a.Blk_ID as BlkId, '; 
	$query .= '  a.Blk_Nummer as PrsNummer, ';
	$query .= '  a.Blk_Omschrijving as BlkOmschrijving ';
	$query .= 'from ';
	$query .= '  oostpoort_blok a ';
	$query .= 'where ';
	$query .= ' a.Blk_Nummer != 0 ';
	$query .= 'order by ';
	$query .= ' a.Blk_Nummer';
  
	//echo $query;  
	$queryResult = db_query($query); 
  
   $page .= '<br/>';
	$page .= '<b>Blok </b>';  
	$page .= '<select name="BlkId">';   
	$page .= '<option value="0" selected="selected">';
		
	while ($data = db_fetch_object($queryResult))
	{
		$page .= '<option value="'.$data->BlkId.'" ';
		$page .= '>Blok '.$data->PrsNummer;
		$page .= '</option>';
	}
	$page .='</select> '; 
	$page .= oostpoort_hiddenlink('OostpoortForm','',0,'Filteren',t('Filteren'));
  
	$page .= '<br/>';
	$page .= '<br/>';
   $page .= t('Selecteer nieuwe adressen om te bezoeken.');

	// Query Database
	$query  = 'select ';
	$query .= '  a.Adr_Id as AdrId, ';
	$query .= '  concat (a.Adr_Straat," ", a.Adr_Huisnr) as Straatnaam, ';
	$query .= '  a.Adr_Plaats as AdrPlaats, ';
	$query .= '  a.Adr_postcode as AdrPostcode, ';
	$query .= '  a.Adr_Telefoon as AdrTelefoon ';
	$query .= 'from ';
	$query .= '  oostpoort_adres a ';
	$query .= 'where ';
	$query .= '  a.Adr_id not in (select b.Bzk_Adr_ID from oostpoort_bezoek b where b.Bzk_BezoekNietGewenstDatum=NULL) ';
	$query .= 'order by ';
	$query .= '  Straatnaam';
  
	$queryResult = db_query($query);
	//echo $query;
  
	// Show Banner
	$page .= '<table>';
	$page .= '<tr>';
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
		$page_tmp .= '<td>'.oostpoort_view_checkbox2( "Active".$i).'</td>';	  
		$page_tmp .= '<td>'.oostpoort_hiddenlink('OostpoortForm', 'AdrId', $data->AdrId, 'Select', $data->Straatnaam).'</td>';
		$page_tmp .= '<td>'.$data->AdrPostcode.'</td>';
		$page_tmp .= '<td>'.$data->AdrPlaats.'</td>';	
		$page_tmp .= '<td>'.$data->AdrTelefoon.'</td>';	
		$page_tmp .= '<td>-</td>';
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
 	$page .= oostpoort_hiddenlink('OostpoortForm','',0,'Select',t('Terug'));
	
	$page .= '</form>';
	$page .= '</div>';

	return $page;  
}


// ##################################################################################
// UTILS
// ##################################################################################

function oostpoort_view_checkbox2( $label ) {

	return '<input type="checkbox" name="'.$label.'" />';
}


// ##################################################################################
// THE END
// ##################################################################################