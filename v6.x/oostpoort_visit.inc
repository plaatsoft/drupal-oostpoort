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

function oostpoort_visit() {
	//echo var_dump($_POST);

   	// Check access for this page.
	if (!oostpoort_check_access()) {
		return drupal_access_denied();
	}
	
	$Action=$_POST['Action'];
	if ($Action=="") {
		$Action=$_SESSION['Action'];
	}
	$_SESSION['Action']=$Action;
	oostpoort_debug_param("Action", $Action);
		
	if ($Action=="Show") {
	
		$page .= oostpoort_visit_view(true);
	
	} else if ($Action=="Edit") {
	
		$page .= oostpoort_visit_view(false);
		
	} else if ($Action=="Cancel") {
		
		$page .= oostpoort_visit_list();
	
	} else if (($Action=="Search")) {

		$page .= oostpoort_visit_search();
	
	} else if ($Action=="Select") {

		$page .= oostpoort_visit_select();
		$page .= oostpoort_visit_list();
	
	} else if ($Action=="VisitCancel") {
	
		$message = oostpoort_visit_cancel();
		if ($message["error"]==0) {
		
			$page .= $message["page"];
			$page .= oostpoort_visit_list();

		} else {
		
			$page .= $message["page"];
			$page .= oostpoort_visit_view(false);
			
		}
	
	} else if ($Action=="VisitReady") {
	
		$message = oostpoort_visit_ready();
		if ($message["error"]==0)
		{
			$page .= $message["page"];
			$page .= oostpoort_visit_list();
			
		} else {
			$page .= $message["page"];
			$page .= oostpoort_visit_view(false);
		}
	
	} else if ($Action=="FamilyView") { 
	  	$page .= oostpoort_family_view(true);
	} else {
		$page = oostpoort_visit_list();
	}
  	if ($page!="") {
		print theme("page", $page);
	}
}

// ##################################################################################
// VIEWS
// ##################################################################################

function oostpoort_visit_list() {
  
	// Breadcrumb menu
	$node["child1"] = t('Bezoekwerk ');
	$page .= oostpoort_breadcrumb($node);
	
    // Get posted parameters
	$MdwId=$_POST["MdwId"];
	if ($MdwId=="") {
		$MdwId=$_SESSION["MdwId"];
		if ($MdwId=="") {
			$MdwId=$_SESSION['MdwId'];
		}
	}	
	$_SESSION["MdwId"]=$MdwId;
	oostpoort_debug_param("MdwId", $MdwId);
	
	// Build page
	$page .= '<div class="oostpoort">';
	
	drupal_set_title(t('Bezoekwerk' ) );

	$page .= '<form name="OostpoortForm" method="POST" >';
  
   //only administrators can switch between visit members.
   if (user_access('access administration pages')) {
  
		$page .= '<fieldset>' ;	
		$page .= '<legend>'.t('Filteren').'</legend>';
		
		$page .= '<div class="form-item">';		
		$page .= '<label>';
		$page .= t('Bezoekmedewerker').': '.oostpoort_view_manitory();
		$page .= '</label>';  
		$page .= oostpoort_view_visiters("MdwId", $MdwId).' ';	
		$page .= oostpoort_hiddenlink('OostpoortForm','','','List',t('Zoek'));			
		$page .= '<div class="description">';
		$page .= t('Select hier de bezoekmedewerker daarna worden alle ingeplande bezoeken getoond.');
		$page .= '</div>';
		$page .= '</div>';
			
		$page .= '</fieldset>' ;		
	} 
	
	$page .= t('Selecteer adres om bezoekverslag te schrijven.');
	
 	// Query Database
	$query  = 'select ';
	$query .= '   b.Adr_Plaats as AdrPlaats, ';
	$query .= '   b.Adr_postcode as AdrPostcode, ';
	$query .= '   concat(b.Adr_Straat," ", b.Adr_Huisnr) as Straatnaam, ';
	$query .= '   a.Bzk_GeplandDatum as BzkGeplande, ';
	$query .= '   a.Bzk_AfgelegdDatum as BzkAfgelegd, ';
	$query .= '   (select concat(Prs_Tussenvoegsels, " ", substring_index(Prs_Achternaam,"-",1)) as naam from oostpoort_persoon where Prs_Huidig_Adr_Id=b.Adr_id limit 1) as Naam, ';
	$query .= '   a.Bzk_ID as BzkId '; 
	$query .= 'from ';
	$query .= '   oostpoort_bezoek a, oostpoort_adres b ';
	$query .= 'where ';
	$query .= '   a.Bzk_Adr_ID=b.Adr_id and ';
	$query .= '   a.Bzk_AfgelegdDatum=0 and ';
	$query .= '   a.Bzk_GeannuleerdDatum=0 and ';
   $query .= '   a.Bzk_BezoekNietGewenstDatum=0 and '; 
	$query .= '   a.Bzk_Mdw_id='.$MdwId.' ';
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
	$page .= '<th><b>'.t('Achternaam').'</b></th>';
	$page .= '<th class="active"><b>'.t('Straat').'</b></th>';
	$page .= '<th><b>'.t('Postcode').'</b></th>';
	$page .= '<th><b>'.t('Plaats').'</b></th>';
	$page .= '<th><b>'.t('Geplande').'</b></th>';
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
		$page_tmp .= '<td>'.oostpoort_convert_date($data->BzkGeplande).'</td>';
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
	
	$page .= oostpoort_hiddenlink('OostpoortForm','MdwId',$MdwId,'Search',t('Bezoeken toevoegen')).' | ';
	$page .= '<a href="'.url(OOSTPOORT_MENU).'">'.t('Terug').'</a>';
	
	$page .= '</form>';
	$page .= '</div>';

	return $page;  
}

function oostpoort_visit_view($readonly) {
  
	// Create Breadcrumb menu
	$page  = '<div class="breadcrumb">';
	$page .= '<a href="'.url("").'">Start</a> > ';
	$page .= '<a href="'.url(OOSTPOORT_MENU).'">Oostpoort</a> > ';
	$page .= 'Bezoekwerk > Bezoek Informatie';
	$page .= '</div>';

    // Get posted parameters
	$BzkId=$_POST["BzkId"];
	oostpoort_debug_param("BzkId", $BzkId);
	
	$AdrId=$_POST["AdrId"];
	oostpoort_debug_param("AdrId", $AdrId);
	
	$page .= '<form name="OostpoortForm" method="POST" >';		
	$page .= '<div class="oostpoort">';
	
	drupal_set_title(t('Bezoek Informatie' ) );
	
	if (!$readonly) {
		
		$query = '
		select 
			a.Bzk_Id as BzkId, 
			b.Adr_Id as AdrId, 
			a.Bzk_Adr_Id as BzkAdrId, 
			a.Bzk_GeplandDatum as BzkGeplandDatum, 
			concat(IF(c.Prs_Roepnaam="",c.Prs_Voornamen, c.Prs_Roepnaam)," ", c.Prs_Tussenvoegsels, " ", c.Prs_Achternaam) as Naam 
		from
			oostpoort_bezoek a, oostpoort_adres b, oostpoort_persoon c 
		where
			a.Bzk_Mdw_ID=c.Prs_id and 
			a.Bzk_Adr_Id = b.Adr_Id and 
			a.Bzk_Id='.$BzkId;
	
		oostpoort_debug_sql($query);
		$queryResult = db_query($query); 
		$data = db_fetch_object($queryResult);
	
		$_SESSION['AdrId']=$data->AdrId;		
		$page .= oostpoort_family_view(true);
		
		$page .= oostpoort_visit_part($data, false);
		
		$AdrId=$data->AdrId;
	
	} else { 
		
		// Readonly view
		$_SESSION['AdrId']=$AdrId;
		
		$page .= oostpoort_family_view(true);
	} 
		
	// Find all other closed visit entries.
	$query  = '
	select 
		a.Bzk_Id as BzkId, 
	 	a.Bzk_BezoekInfo as BzkInfo, 
	 	a.Bzk_GeplandDatum as BzkGeplandDatum, 
		a.Bzk_AfgelegdDatum as BzkAfgelegdDatum, 
		a.Bzk_GeannuleerdDatum as BzkGeannuleerdDatum, 
   		a.Bzk_BezoekNietGewenstDatum as BzkBezoekNietGewenstDatum, 
		a.Bzk_Bzt_ID as BzkBztId, 
		a.Bzk_Adr_Id as BzkAdrId, 
		concat(IF(b.Prs_Roepnaam="",b.Prs_Voornamen, b.Prs_Roepnaam)," ", b.Prs_Tussenvoegsels, " ", b.Prs_Achternaam) as Naam 
	from 
		oostpoort_bezoek a, oostpoort_persoon b 
	where 
		a.Bzk_Mdw_ID=b.Prs_id and 
		a.Bzk_Adr_Id='.$AdrId.' and 
		(a.Bzk_AfgelegdDatum!=0 or 
		a.Bzk_GeannuleerdDatum!=0 or 
   		a.Bzk_BezoekNietGewenstDatum!=0)  
	order by 
		a.Bzk_Id desc ';
	
   	oostpoort_debug_sql($query);
	$queryResult = db_query($query); 
	while ($data = db_fetch_object($queryResult)) {
		$page .= oostpoort_visit_part($data, true);
	}
	
	if ($readonly) {
		$page .= oostpoort_hiddenlink('OostpoortForm','MdwId' ,$MdwId,'Search',t('Terug'));	
	} else {
		$page .= oostpoort_hiddenlink('OostpoortForm','',0,'Cancel',t('Terug'));	
	}
		
   	$page .= '</div>';
   
   	if ($readonly) {
   		// Repost selected address previous screen.
    	$page.=oostpoort_keep_selection();
   	}
   
	$page .= '</form>';
	
   return $page;  
}

function oostpoort_visit_part($data, $readonly)
{		
	$VisitDate=$_POST["VisitDate"];
	oostpoort_debug_param("VisitDate", $VisitDate);
	
	$BzkInfo=$_POST["BzkInfo"];
	if ($BzkInfo=="") {
		$BzkInfo=$data->BzkInfo;
	}
	oostpoort_debug_param("BzkInfo", $BzkInfo);
	
	$VisitType=$_POST["VisitType"];
	if ($VisitType=="") {
		$VisitType=$data->BzkBztId;
	}
	oostpoort_debug_param("VisitType", $VisitType);

	$page .= '<fieldset>';
	
	if ($data->BzkBezoekNietGewenstDatum!=0) {
		$page .= '<legend>';
		$page .= 'Bezoekt niet gewenst op '.oostpoort_convert_date($data->BzkBezoekNietGewenstDatum);
		$page .= ' [Gepland op '.oostpoort_convert_date($data->BzkGeplandDatum).']';
		$page .= '</legend>';
		$type = 2;
	} else if ($data->BzkAfgelegdDatum!=0) {
		$page .= '<legend>';
		$page .= 'Bezoek afgelegd op '.oostpoort_convert_date($data->BzkAfgelegdDatum);
		$page .= ' [Gepland op '.oostpoort_convert_date($data->BzkGeplandDatum).']';
		$page .= '</legend>';
		$type = 1;
	} else if ($data->BzkGeannuleerdDatum!=0) {
		$page .= '<legend>';
		$page .= 'Bezoek geannuleerd op '.oostpoort_convert_date($data->BzkGeannuleerdDatum);
		$page .= ' [Gepland op '.oostpoort_convert_date($data->BzkGeplandDatum).']';
		$page .= '</legend>';
		$type = 3;
	} else {
   		$page .= '<legend>';
   		$page .= 'Geboekt op '.oostpoort_convert_date($data->BzkGeplandDatum);
   		$page .= '</legend>';
		$type = 4;
	}
		
	$page .= '<div class="form-item">';
	$page .= '<label>'.t('Bezoek ID').': </label>';
	$page .= $data->BzkId;
	$page .= '</div>';
	
	$page .= '<div class="form-item">';
	$page .= '<label>'.t('Bezoekmedewerker').': </label>';
	$page .= $data->Naam;
	$page .= '</div>';
	
	if ($type==4)  {	
		$page .= '<div class="form-item">';
		$page .= '<label>'.t('Datum').': <span class="form-required">*</span></label>';
		$page .= oostpoort_view_date("VisitDate", $VisitDate, $readonly);
		$page .= '<div class="description">';
		$page .= t('Datum wanneer bezoek is uitgevoerd of geannuleerd.');
		$page .= '</div>';
		$page .= '</div>';
	}
	
	if (($type==1) or ($type==3) or ($type==4)) {
		$page .= '<div class="form-item">';
		$page .= '<label>'.t('Bezoek type').': <span class="form-required">*</span></label>';
		$page .= oostpoort_view_type("VisitType", $VisitType, $readonly);
		$page .= '<div class="description">';
		if ($type==4)  {
			$page .= t('Selecteer wat voor type bezoek is uitgevoerd.');
		}
		$page .= '</div>';
		$page .= '</div>';
	}
	
	$page .= '<div class="form-item">';
	$page .= '<label>'.t('Notities').': <span class="form-required">*</span></label>';
	$page .= oostpoort_view_note("BzkInfo", $BzkInfo, $readonly);
	$page .= '<div class="description">';
	if ($type==4) {
		$page .= t('Hier kunt u algemene niet vertrouwelijke informatie over het bezoek vastleggen.');
	}
	$page .= '</div>';
	$page .= '</div>';
	
		if (($type==2) or ($type==4)) {
		$page .= '<div class="form-item">';
		$page .= '<label>'.t('Bezoek niet gewenst').': </label>';
		if ($readonly) {
			$page .= oostpoort_view_checkbox("","on",true);
		} else {
			$page .= oostpoort_view_checkbox("NoVisit",$_POST["NoVisit"],$readonly);
		}
		$page .= '<div class="description">';
		if ($type==4) {
			$page .= t('Als bezoek niet meer gewenst is selecteer deze optie. Er zullen dan geen bezoeken meer worden ingepland.');
		}
		$page .= '</div>';
		$page .= '</div>';
	}
	
	if (($type==1) or ($type==4)) {
	
		// Get Address members.
	
		if (!$readonly) {
			
			$query  = 'select ';
			$query .= '  a.Prs_Id as PrsId, ';
			$query .= '  concat(IF(a.Prs_Roepnaam="",a.Prs_Voorletters, a.Prs_Roepnaam)," ", a.Prs_Tussenvoegsels, " ", a.Prs_Achternaam) as Naam, ';
			$query .= '  a.Prs_GeslachtMV as PrsGeslacht, '; 
			$query .= '  a.Prs_Geboortedatum as PrsGeboorteDatum, ';
			$query .= '  a.Prs_Huwelijksdatum as PrsHuwelijksdatum ';
			$query .= 'from ';
			$query .= '  oostpoort_persoon a ';
			$query .= 'where ';
			$query .= '  a.Prs_Huidig_Adr_ID='.$data->BzkAdrId.' ';
			$query .= 'order by ';
			$query .= '  PrsGeboorteDatum';
			
		} else {
		
			$query  = 'select ';
			$query .= '  a.Prs_Id as PrsId, ';
			$query .= '  concat(IF(a.Prs_Roepnaam="",a.Prs_Voorletters, a.Prs_Roepnaam)," ", a.Prs_Tussenvoegsels, " ", a.Prs_Achternaam) as Naam, ';
			$query .= '  a.Prs_GeslachtMV as PrsGeslacht, '; 
			$query .= '  a.Prs_Geboortedatum as PrsGeboorteDatum, ';
			$query .= '  a.Prs_Huwelijksdatum as PrsHuwelijksdatum ';
			$query .= 'from ';
			$query .= '  oostpoort_persoon a, oostpoort_bezoekpersoon b ';
			$query .= 'where ';
			$query .= '  b.Bzp_Bzk_ID='.$data->BzkId.' and ';
			$query .= '  a.Prs_Id = b.Bzp_Prs_ID ';
			$query .= 'order by ';
			$query .= '  PrsGeboorteDatum';
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
			if ($readonly) {
				$page .= '<td>'.oostpoort_view_checkbox( "" , "on" , true).'</td>';						
			} else {
				$page .= '<td>'.oostpoort_view_checkbox( "Active-".$persoon->PrsId, $_POST["Active-".$persoon->PrsId], $readonly).'</td>';
			}
			$page .= '<td>'.$persoon->Naam.'</td>';
			$page .= '<td>'.oostpoort_view_geslacht($persoon->PrsGeslacht).'</td>';
			$page .= '<td class="active">'.oostpoort_convert_date($persoon->PrsGeboorteDatum).'</td>';
			$page .= '<td>'.oostpoort_convert_date($persoon->PrsHuwelijksdatum).'</td>';
			$page .= '<td>'.$persoon->PrsMobiel.'</td>';
			$page .= '<td>'.$persoon->PrsEmail.'</td>';
			$page .= '</tr>';
		}
		$page .= '</tbody>';
		$page .= '</table>';

		// Button bar	
		if ($type==4)
		{
			$page .= oostpoort_hiddenlink('OostpoortForm','BzkId',$data->BzkId,'VisitReady',t('Bezoek afronden')).' | ';
			$page .= oostpoort_hiddenlink('OostpoortForm','BzkId',$data->BzkId,'VisitCancel',t('Bezoek annuleren'));
		}
	}
		
	$page .= '</fieldset>' ;
	
	return $page;
}

function oostpoort_get_block($MdwId) {
	
	// Query Database
	$query  = '
		select 
			a.Blk_Id as BlkId 
		from 
			oostpoort_blok a, oostpoort_medewerker b
		where
			b.Mdw_Blk_ID=a.Blk_ID and
			b.Mdw_Id='.$MdwId;
	
	oostpoort_debug_sql($query);
	$queryResult = db_query($query);
  	$result = db_fetch_object($queryResult);
  	
  	$BlkId=$result->BlkId;	
  	return $BlkId;
}


function oostpoort_visit_search() {
  
	// Create Breadcrumb menu
	$page  = '<div class="breadcrumb">';
	$page .= '<a href="'.url("").'">Start</a> > ';
	$page .= '<a href="'.url(OOSTPOORT_MENU).'">Oostpoort</a> > ';
	$page .= 'Bezoekwerk > Bezoeken plannen';
	$page .= '</div>';
	
	// Not use on this moment
	$MdwId=$_SESSION["MdwId"];
	oostpoort_debug_param("MdwId", $MdwId);
 	
	$BlkId=oostpoort_get_block($MdwId);
	
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
	$page .= '<input id="text" name="Search" size="40" maxlength="40" type="text" value="'.$Search.'"> ';
	$page .= oostpoort_hiddenlink('OostpoortForm','','','Search',t('Zoek'));
	$page .= '<div class="description">';
	$page .= t('Filteren op straatnaam.');
	$page .= '</div>';
	$page .= '</div>';
	
	$page .= oostpoort_sortbar($Sort,"Search");
	
	$page .= '</fieldset>' ;
  
   	$page .= t('Selecteer nieuwe adressen om te bezoeken.');

	// Query Database
	$query  = '
	
	select 
    	a.Adr_id as AdrId,      
    	concat(a.Adr_Straat," ", a.Adr_Huisnr) as Straatnaam, 
    	a.Adr_Plaats as AdrPlaats, 
    	a.Adr_postcode as AdrPostcode, 
    	a.Adr_Telefoon as AdrTelefoon,  
    	a.Adr_Blk_Id as AdrBlkId,
    	(select concat(Prs_Tussenvoegsels, " ", substring_index(Prs_Achternaam,"-",1)) from oostpoort_persoon where Prs_Huidig_Adr_Id=a.Adr_id limit 1) as Naam, 
    	(select Bzk_Id from oostpoort_bezoek where Bzk_Adr_Id=a.Adr_Id order by Bzk_Id desc limit 1) as BzkId,
 		(select Bzk_GeplandDatum from oostpoort_bezoek where Bzk_id=BzkId) as BzkGeplandDatum,
		(select Bzk_AfgelegdDatum from oostpoort_bezoek where Bzk_id=BzkId) as BzkAfgelegdDatum ,
		(select Bzk_GeannuleerdDatum from oostpoort_bezoek where Bzk_id=BzkId) as BzkGeannuleerdDatum,
		(select Bzk_BezoekNietGewenstDatum from oostpoort_bezoek where Bzk_id=BzkId) as BzkBezoekNietGewenstDatum,
		(select concat(IF(b.Prs_Roepnaam="",b.Prs_Voorletters, b.Prs_Roepnaam), " ", b.Prs_Tussenvoegsels, " ", b.Prs_Achternaam) from oostpoort_persoon b, oostpoort_bezoek c where c.Bzk_id=BzkId and b.prs_id=c.bzk_mdw_id) as BzkBezoekPersoon
	from 
		oostpoort_adres a  
	where ';
	if 	($BlkId!=0) {
		$query .= 'a.Adr_Blk_ID='.$BlkId.' and ';
	}
	if (strlen($Search)==0) {
		$query .= 'a.Adr_Straat LIKE "'.$Sort.'%" ';
	} else { 
		$query .= 'a.Adr_Straat LIKE "'.$Search.'%" ';
 	}
	$query .= '
		order by 
			Straatnaam'; 
  
   	oostpoort_debug_sql($query);
	$queryResult = db_query($query);
  
	// Show Banner
	$page .= '<table>';
	$page .= '<thead>';
	$page .= '<tr>';
	$page .= '<th><b>'.t('').'</b></th>';
	$page .= '<th><b>'.t('Achternaam').'</b></th>';
	$page .= '<th class="active"><b>'.t('Straat').'</b></th>';
	$page .= '<th><b>'.t('Postcode').'</b></th>';
	$page .= '<th><b>'.t('Plaats').'</b></th>';
	$page .= '<th><b>'.t('Blok').'</b></th>';
	$page .= '<th><b>'.t('Opmerking').'</b></th>';
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
		if ( (($data->BzkGeplandDatum!=0) && 
		     ($data->BzkAfgelegdDatum==0) &&
		     ($data->BzkGeannuleerdDatum==0)) || 
		     ($data->BzkBezoekNietGewenstDatum!=0)) {
			$page_tmp .= '<td></td>';
		} else {
			$page_tmp .= '<td>'.oostpoort_view_checkbox( "Active-".$data->AdrId, $_POST["Active-".$data->AdrId], false).'</td>';	  
		} 
		$page_tmp .= '<td>'.$data->Naam."</td>";
		$page_tmp .= '<td class="active">'.oostpoort_hiddenlink('OostpoortForm', 'AdrId', $data->AdrId, 'Show', $data->Straatnaam)."</td>";
		$page_tmp .= '<td>'.$data->AdrPostcode.'</td>';
		$page_tmp .= '<td>'.$data->AdrPlaats.'</td>';	
		$page_tmp .= '<td>'.$data->AdrBlkId.'</td>';	
		
		$page_tmp .= '<td>';
		$page_tmp .= '<div class="description">';
		if ($data->BzkBezoekNietGewenstDatum!=0) {
			$page_tmp.=t('Bezoek niet gewenst').'<br/>';
			$page_tmp.=oostpoort_convert_date($data->BzkBezoekNietGewenstDatum).'<br/>';
			$page_tmp.= $data->BzkBezoekPersoon;
		} else if ($data->BzkAfgelegdDatum!=0) {
			$page_tmp.=t('Afgelegde bezoek').'<br/>';
			$page_tmp.=oostpoort_convert_date($data->BzkAfgelegdDatum).'<br/>';
			$page_tmp.= $data->BzkBezoekPersoon;
		} else if ($data->BzkGeannuleerdDatum!=0) {
			$page_tmp.=t('Geannuleerd bezoek').'<br/>';
			$page_tmp.=oostpoort_convert_date($data->BzkGeannuleerdDatum).'<br/>';
			$page_tmp.= $data->BzkBezoekPersoon;
		} else if ($data->BzkGeplandDatum!=0) {
			$page_tmp.=t('Ingepland bezoek').'<br/>';
			$page_tmp.=oostpoort_convert_date($data->BzkGeplandDatum).'<br/>';
			$page_tmp.= $data->BzkBezoekPersoon;
		} else 
		$page_tmp .= '</div>';
		$page_tmp .= '</td>';
		
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
 	$page .= oostpoort_hiddenlink('OostpoortForm','',0,'Select',t('Selecteren')).' | ';
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
    $MdwId=$_SESSION["MdwId"];
	oostpoort_debug_param("MdwId", $MdwId);
	
	foreach( $_POST as $var => $value) {
			
		list($active, $AdrId)=split("-",$var);
		if (($active=="Active") && ($value=="on"))
		{
			oostpoort_debug_param("AdrId", $AdrId);
			
			// Check if entry all exist (Protect against reload button), if so skip insert)
			$query  = '
				select 
					Bzk_GeplandDatum as BzkGeplandDatum 
				from 
					oostpoort_bezoek 
				where 
					Bzk_Adr_id='.$AdrId.' and
					Bzk_AfgelegdDatum=0 and
					Bzk_GeannuleerdDatum=0 and 
					Bzk_BezoekNietGewenstDatum=0
				order by 
					Bzk_Id desc
				limit 1';
			
			$queryResultTmp = db_query($query);
			$tmp = db_fetch_object($queryResultTmp);
			if ($tmp->BzkGeplandDatum==0)
			{			
				$amount++;
				
				// Insert new visit
				$query = 'INSERT INTO oostpoort_bezoek (';
				$query .= '  `Bzk_Mdw_ID`, `Bzk_Adr_ID`, `Bzk_GeplandDatum`, `Bzk_Bzt_ID`)';
				$query .= 'VALUES ';
				$query .= '( '.$MdwId.','.$AdrId.', "'.date("Y-m-d").'", 1 )';
				
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


function oostpoort_visit_cancel()
{	
	// *****************************
	// Get posted Inputs
	// *****************************
	
	$BzkId=$_POST["BzkId"];
	oostpoort_debug_param("BzkId", $BzkId);
	
	$BzkInfo=$_POST["BzkInfo"];
	oostpoort_debug_param("BzkInfo", $BzkInfo);
	
	$CancelDate=$_POST["VisitDate"];
	oostpoort_debug_param("VisitDate", $VisitDate);
	
	// *****************************
	// Input Validation
	// *****************************

	$page = '<div class="messages error">';
	$page .= '<ul>';
	
	if ($BzkInfo=="") {
		$page .= '<li>';
		$page .= t('Annuleer notitie is verplicht');
		$page .= '</li>';
		$message["error"]=1;
	}

	// Validate Cancel Date
	if ($CancelDate=="")
	{
		$page .= '<li>';
		$page .= t('Annuleer datum is verplicht');
		$page .= '</li>';
		$message["error"]=1;
	} else {
	
		list($day, $month, $year) = split("-",$CancelDate);
		if (!checkdate($month,$day,$year))  {
			$page .= '<li>';
			$page .= t('Annuleer datum is incorrect!');
			$page .= '</li>';
			$message["error"]=1;
		
		} else if (mktime(0,0,0,$month,$day,$year)>mktime()) {
		
			$page .= '<li>';
			$page .= t('Annuleer datum ligt in de toekomst!');
			$page .= '</li>';
			$message["error"]=1;
		}
	}
	$page .= '</ul>';
	$page .= '</div>';
		
	if ($message["error"]==1) {
	
		$message["page"]=$page;
		$message["error"]=1;
		return $message;
	} else {
	   $page="";
	}
	
	// *****************************
	// Data ok, update database
	// *****************************
	
	// Adapt plan date if visit date is before plan date
	$query  = 'select Bzk_GeplandDatum as BzkGeplandDatum from oostpoort_bezoek where Bzk_ID='.$BzkId;
	oostpoort_debug_sql($query);
	$queryResult = db_query($query);
	$tmp = db_fetch_object($queryResult);
	
	$adaptedPlanDate=false;		
	list($BzkYear, $BzkMonth, $BzkDay) = split("-",$tmp->BzkGeplandDatum);
	
	$planDate = mktime(0,0,0,$Bzkmonth,$Bzkday,$Bzkyear);
	
	list($day, $month, $year) = split("-",$CancelDate);
	$cancelDate = mktime(0,0,0,$month,$day,$year);
		
	if ( $planDate>$cancelDate ) {
		$adaptedPlanDate=true;	
	}
	
	$query  = 'UPDATE oostpoort_bezoek SET ';
	$query .= '  Bzk_GeannuleerdDatum="'.oostpoort_convert_date_reverse($CancelDate).'", ';
	if ($adaptedPlanDate) {
		$query .= '  Bzk_GeplandDatum="'.oostpoort_convert_date_reverse($CancelDate).'", ';
	}
	$query .= '  Bzk_BezoekInfo="'.$BzkInfo.'" ';
	$query .= 'where ';
	$query .= '  Bzk_ID='.$BzkId;
	
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
	// *****************************
	// Get posted Inputs
	// *****************************
   
	$BzkId=$_POST["BzkId"];
	oostpoort_debug_param("BzkId", $BzkId);
	
	$NoVisit=$_POST["NoVisit"];
	oostpoort_debug_param("NoVisit", $NoVisit);

   	$VisitType=$_POST["VisitType"];
	oostpoort_debug_param("VisitType", $VisitType);

	$BzkInfo=$_POST["BzkInfo"];
	oostpoort_debug_param("BzkInfo", $BzkInfo);
	
	$VisitDate=$_POST["VisitDate"];
	oostpoort_debug_param("VisitDate", $VisitDate);
	
	// *****************************
	// Input Validation
	// *****************************
	
	$page = '<div class="messages error">';
	$page .= '<ul>';
	
	// Validate Note
	if ($BzkInfo=="")
	{
		$page .= '<li>';
		$page .= t('Bezoek notitie is verplicht');
		$page .= '</li>';
		$message["error"]=1;
	}

    // Validate VisitDate
	if ($VisitDate=="")
	{
		$page .= '<li>';
		$page .= t('Bezoek datum is verplicht');
		$page .= '</li>';
		$message["error"]=1;
	} else {
	
		list($day, $month, $year) = split("-",$VisitDate);
		if (!checkdate($month,$day,$year))  {
			$page .= '<li>';
			$page .= t('Bezoekdatum is incorrect!');
			$page .= '</li>';
			$message["error"]=1;
		
		} else if (mktime(0,0,0,$month,$day,$year)>mktime()) {
		
			$page .= '<li>';
			$page .= t('Bezoekdatum ligt in de toekomst!');
			$page .= '</li>';
			$message["error"]=1;
			
		}
	}
	
	// Validate Member selection
	$amount=0;
	foreach( $_POST as $var => $value) {
			
		list($active, $PrsId)=split("-",$var);
		if (($active=="Active") && ($value=="on")) {
			$amount++;
		}
	}
	if ($amount=="")
	{
		$page .= '<li>';
		$page .= t('Geen personen geselecteerd');
		$page .= '</li>';
		$message["error"]=1;
	}
		
	$page .= '</ul>';
	$page .= '</div>';
		
	if ($message["error"]==1) {
		$message["page"]=$page;
		$message["error"]=1;
		return $message;
	}

	// *****************************
	// Data ok, update database
	// *****************************
	
	// Adapt plan date if visit date is before plan date
	$query  = 'select Bzk_GeplandDatum as BzkGeplandDatum from oostpoort_bezoek where Bzk_ID='.$BzkId;
	oostpoort_debug_sql($query);
	$queryResult = db_query($query);
	$tmp = db_fetch_object($queryResult);
	
	$adaptedPlanDate=false;		
	list($BzkYear, $BzkMonth, $BzkDay) = split("-",$tmp->BzkGeplandDatum);
	$planDate = mktime(0,0,0,$Bzkmonth,$Bzkday,$Bzkyear);
	
	list($day, $month, $year) = split("-",$VisitDate);
	$visitDate = mktime(0,0,0,$month,$day,$year);
	
	if ( $planDate>$visitDate ) {
		$adaptedPlanDate=true;	
	}
	
	// Update database
	$query  = 'UPDATE oostpoort_bezoek SET ';
	$query .= '  Bzk_BezoekInfo="'.$BzkInfo.'", ';
	if ($NoVisit=="on") {
	   $query .= '  Bzk_BezoekNietGewenstDatum="'.oostpoort_convert_date_reverse($VisitDate).'", ';	
	} else {
		$query .= '  Bzk_AfgelegdDatum="'.oostpoort_convert_date_reverse($VisitDate).'", ';
	}
	if ($adaptedPlanDate) {
		$query .= '  Bzk_GeplandDatum="'.oostpoort_convert_date_reverse($VisitDate).'", ';
	}
	$query .= '  Bzk_Bzt_ID="'.$VisitType.'" ';	
	$query .= 'where ';
	$query .= '  Bzk_ID='.$BzkId;
		
	oostpoort_debug_sql($query);
	db_query($query); 

	// Check if entry all exist (Protect against reload button), if so skip insert
	$query  = 'select count(*) as Amount from oostpoort_bezoekpersoon where Bzp_Bzk_ID='.$BzkId.';';
	oostpoort_debug_sql($query);
	$queryResult = db_query($query);
	$tmp = db_fetch_object($queryResult);
	if ($tmp->Amount==0) {			
	
		foreach( $_POST as $var => $value) {
			
			list($active, $PrsId)=split("-",$var);
			if (($active=="Active") && ($value=="on")) {
			
				oostpoort_debug_param("PrsId", $PrsId);
			
				// Insert new visit
				$query = 'INSERT INTO oostpoort_bezoekpersoon (';
				$query .= '  `Bzp_Bzk_ID`, `Bzp_Prs_ID`) ';
				$query .= 'VALUES ';
				$query .= '( '.$BzkId.','.$PrsId.' ); ';
		
				oostpoort_debug_sql($query);
				db_query($query);		
			}
		}
		$page = '<div class="messages status">';
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