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

/**
 * oostpoort_visit
 *
 * State machine of visit member page.
 *
 * @return The HTML code
 *
 * @author wplaat
 */
function oostpoort_visit() {
	
	//echo var_dump($_POST);
	
	// Check access for this page.
	if (!oostpoort_visit_member_access()) {
		return drupal_access_denied();
	}
	
	// *****************************
	// Process POST variables
	// *****************************
	
	$Action=htmlentities($_POST['Action']);
	oostpoort_debug_param("Action", $Action);
		
	// *****************************
	// Process statemachine
	// *****************************
	
	switch ($Action) {
	
		case 'VisitShow':  
			$page .= oostpoort_visit_view(true);
			break;
			
		case 'VisitEdit':	
			$page .= oostpoort_visit_view(false);
			break;
						
		case 'VisitSearch':
			$page .= oostpoort_visit_select();
			break;
		
		case 'VisitSelect':
			oostpoort_action_select();
			$page .= oostpoort_visit_list();
			break;
		
		case 'VisitDeselect':
			oostpoort_action_deselect();
			$page .= oostpoort_visit_list();
			break;
			
		case 'VisitCancel':				
			$error = oostpoort_action_cancel();
			if ($error==0) {		
				$page .= oostpoort_visit_list();
			} else {		
				$page .= oostpoort_visit_view(false);			
			}
			break;
			
		case 'VisitReady':
			$error = oostpoort_action_ready();
			if ($error==0) {
				$page .= oostpoort_visit_list();
			} else {		
				$page .= oostpoort_visit_view(false);
			}
			break;
		
		
		case 'Cancel':		
		default: 
			$page = oostpoort_visit_list();
			break;
	}
	print theme("page", $page);
}

// ##################################################################################
// VIEWS
// ##################################################################################

/**
 * oostpoort_visit_list
 *
 * Show all visits with one visit member has executed and planned.
 *
 * @return The HTML code
 *
 * @author wplaat
 */
function oostpoort_visit_list() {
 
 	// *****************************
	// Get posted Inputs
	// *****************************
	
	$MdwId=htmlentities($_POST["MdwId"]);
	if ($MdwId=="") {
		$MdwId=$_SESSION["MdwId"];
		if ($MdwId=="") {
			$MdwId=$_SESSION['MdwId'];
		}
	}	
	$_SESSION["MdwId"]=$MdwId;
	oostpoort_debug_param("MdwId", $MdwId);
	
	// *****************************
	// Build page
	// *****************************
	
	$page .= '<form name="OostpoortForm" method="POST" >';
	$page .= '<div class="oostpoort">';
	
	// Breadcrumb menu
	$node["child1"] = l(t('Bezoekwerk'),OOSTPOORT_VISIT);
	oostpoort_breadcrumb($node);
	
	// title
	drupal_set_title(t('Bezoekwerk' ) );

   //only visit member admins can switch between visit members.
   if (oostpoort_visit_member_admin_access()) {
  
		$page .= '<fieldset>' ;	
		$page .= '<legend><i>'.t('Filteren').'</i></legend>';
		
		$page .= '<div class="form-item">';		
		$page .= '<label>';
		$page .= t('Bezoekmedewerker').': '.oostpoort_view_manitory();
		$page .= '</label>';  
		$page .= oostpoort_view_visiters("MdwId", $MdwId).' ';	
		$page .= oostpoort_hiddenlink('OostpoortForm','','','List',t('Zoek'));			
		$page .= '<div class="description">';
		$page .= t('Selecteer hier de bezoekmedewerker daarna worden alle ingeplande bezoeken getoond.');
		$page .= '</div>';
		$page .= '</div>';
			
		$page .= '</fieldset>' ;		
	} 
	
	// ***************************************************************
	
	$page .= '<fieldset>';	
	$page .= '<legend><i>'.t('Ingeplande bezoeken').'</i></legend>';
	$page .= t('Selecteer een adres om een bezoekverslag te schrijven.');
	
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
	$query .= '   a.Bzk_ID asc';
   oostpoort_debug_sql($query);
	$queryResult = db_query($query);
  
	// Show Banner
	$page .= '<table>';
	$page .= '<tr>';
	$page .= '<thead>';
	$page .= '<tr>';
	$page .= '<th><b>'.t('').'</b></th>';
	$page .= '<th class="active"><b>'.t('Bezoek Id').'</b></th>';
	$page .= '<th><b>'.t('Achternaam').'</b></th>';
	$page .= '<th><b>'.t('Straat').'</b></th>';
	$page .= '<th><b>'.t('Postcode').'</b></th>';
	$page .= '<th><b>'.t('Plaats').'</b></th>';
	$page .= '<th><b>'.t('Overige').'</b></th>';
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
		$page_tmp .= '<td>'.oostpoort_view_checkbox( "Active-".$data->BzkId, $_POST["Active-".$data->BzkId], false).'</td>';	  
		$page_tmp .= '<td class="active">'.oostpoort_hiddenlink('OostpoortForm', 'BzkId', $data->BzkId, 'VisitEdit', $data->BzkId).'</td>';	  
		$page_tmp .= '<td>'.$data->Naam.'</td>';	  
		$page_tmp .= '<td>'.$data->Straatnaam.'</td>';
		$page_tmp .= '<td>'.$data->AdrPostcode.'</td>';
		$page_tmp .= '<td>'.$data->AdrPlaats.'</td>';	
		
		$page_tmp .= '<td>';
		$page_tmp .= '<div class="description">';
		$page_tmp.=t('Ingepland bezoek').'<br/>';
		$page_tmp .= oostpoort_convert_date($data->BzkGeplande);
		$page_tmp .= '</div>';
		$page_tmp .= '<td>';
		
		$page_tmp .= '</tr>';
   }

	if ( $page_tmp!='') {
		$page .= $page_tmp;
	} else  {	
		$page .= '<tr class="even">';
		$page .= '<td>'.t('Geen bezoeken gevonden').'</td>';
		$page .= '</tr>';
	}
	$page .= '</tbody>';
	$page .= '</table>';
	
	$page .= oostpoort_hiddenlink('OostpoortForm','MdwId',$MdwId,'VisitSearch',t('Bezoeken toevoegen'));
	if ( $page_tmp!='') {
		// If data show deselect action
		$page .= ' | '.oostpoort_hiddenlink('OostpoortForm','MdwId',$MdwId,'VisitDeselect',t('Bezoeken verwijderen'));
	}
	
	$page .= '</fieldset>' ;	
	
	// **********************************************
	
	$page .= '<fieldset>';	
	$page .= '<legend><i>'.t('Uitgevoerde bezoeken').'</i></legend>';
	$page .= t('Selecteer een adres om een bezoekverslag aan te passen.');
	
	// Query Database
	$query  = 'select ';
	$query .= '   b.Adr_Plaats as AdrPlaats, ';
	$query .= '   b.Adr_postcode as AdrPostcode, ';
	$query .= '   concat(b.Adr_Straat," ", b.Adr_Huisnr) as Straatnaam, ';
	$query .= '   a.Bzk_AfgelegdDatum as BzkAfgelegdDatum, ';
	$query .= '   a.Bzk_GeannuleerdDatum as BzkGeannuleerdDatum, ';
	$query .= '   a.Bzk_BezoekNietGewenstDatum as BzkBezoekNietGewenstDatum, ';
	$query .= '   (select concat(Prs_Tussenvoegsels, " ", substring_index(Prs_Achternaam,"-",1)) as naam from oostpoort_persoon where Prs_Huidig_Adr_Id=b.Adr_id limit 1) as Naam, ';
	$query .= '   a.Bzk_ID as BzkId '; 
	$query .= 'from ';
	$query .= '   oostpoort_bezoek a, oostpoort_adres b ';
	$query .= 'where ';
	$query .= '   a.Bzk_Adr_ID=b.Adr_id and ';
	$query .= '   (a.Bzk_AfgelegdDatum!=0 or ';
	$query .= '   a.Bzk_GeannuleerdDatum!=0 or ';
   $query .= '   a.Bzk_BezoekNietGewenstDatum!=0) and '; 
	$query .= '   a.Bzk_Mdw_id='.$MdwId.' ';
	$query .= 'order by ';
	$query .= '   a.Bzk_ID desc';
   oostpoort_debug_sql($query);
	$queryResult = db_query($query);
  
	// Show Banner
	$page .= '<table>';
	$page .= '<tr>';
	$page .= '<thead>';
	$page .= '<tr>';
	$page .= '<th class="active"><b>'.t('Bezoek Id').'</b></th>';
	$page .= '<th><b>'.t('Achternaam').'</b></th>';
	$page .= '<th><b>'.t('Straat').'</b></th>';
	$page .= '<th><b>'.t('Postcode').'</b></th>';
	$page .= '<th><b>'.t('Plaats').'</b></th>';
	$page .= '<th><b>'.t('Overige').'</b></th>';
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
		$page_tmp .= '<td class="active">'.oostpoort_hiddenlink('OostpoortForm', 'BzkId', $data->BzkId, 'VisitEdit', $data->BzkId).'</td>';	  
		$page_tmp .= '<td>'.$data->Naam.'</td>';	  
		$page_tmp .= '<td>'.$data->Straatnaam.'</td>';
		$page_tmp .= '<td>'.$data->AdrPostcode.'</td>';
		$page_tmp .= '<td>'.$data->AdrPlaats.'</td>';	
		
		$page_tmp .= '<td>';
		$page_tmp .= '<div class="description">';
	
		if ($data->BzkBezoekNietGewenstDatum!=0) {
		
			$page_tmp.=t('Niet gewenst bezoek').'<br/>';
			$page_tmp.=oostpoort_convert_date($data->BzkBezoekNietGewenstDatum);
			
		} else if ($data->BzkAfgelegdDatum!=0) {
		
			$page_tmp.=t('Afgelegde bezoek').'<br/>';
			$page_tmp.=oostpoort_convert_date($data->BzkAfgelegdDatum);
			
		} else if ($data->BzkGeannuleerdDatum!=0) {
		
			$page_tmp.=t('Geannuleerd bezoek').'<br/>';
			$page_tmp.=oostpoort_convert_date($data->BzkGeannuleerdDatum);
		} 
		$page_tmp .= '</div>';
		$page_tmp .= '</td>';	
		
		$page_tmp .= '</tr>';
   }

	if ( $page_tmp!='') {
		$page .= $page_tmp;
	} else  {	
		$page .= '<tr class="even">';
		$page .= '<td>'.t('Geen bezoeken gevonden').'</td>';
		$page .= '</tr>';
	}
	$page .= '</tbody>';
	$page .= '</table>';
	
	$page .= '</fieldset>';	
	
	$page .= '<a href="'.url(OOSTPOORT_MENU).'">'.t('Terug').'</a>';
	
	$page .= '</form>';
	$page .= '</div>';

	return $page;  
}

/**
 * oostpoort_visit_view
 *
 * Show all visit reports in detail.
 * 
 * @param readonly	The ready only flag.
 * 
 * @return The created html data.
 *
 * @author wplaat
 */
function oostpoort_visit_view($readonly) {
  
   // *****************************
	// Get posted Inputs
	// *****************************
	
	$BzkId=htmlentities($_POST["BzkId"]);
	oostpoort_debug_param("BzkId", $BzkId);
	
	$AdrId=htmlentities($_POST["AdrId"]);
	oostpoort_debug_param("AdrId", $AdrId);
	
	// *****************************
	// Build page
	// *****************************
	
	$page .= '<form name="OostpoortForm" method="POST" >';	
	$page .= '<div class="oostpoort">';
	
	// Breadcrumb menu
	$node["child1"] = l(t('Bezoekwerk'),OOSTPOORT_VISIT);
	$node["child2"] = t('Bezoek informatie');
	oostpoort_breadcrumb($node);
	
	drupal_set_title(t('Bezoek Informatie' ) );
		
	if (!$readonly) {
		
		$query = '
		select 
			a.Bzk_Id as BzkId, 
			a.Bzk_BezoekInfo as BzkInfo, 
			a.Bzk_GeplandDatum as BzkGeplandDatum, 
			a.Bzk_AfgelegdDatum as BzkAfgelegdDatum, 
			a.Bzk_GeannuleerdDatum as BzkGeannuleerdDatum, 
			a.Bzk_BezoekNietGewenstDatum as BzkBezoekNietGewenstDatum, 
			b.Adr_Telefoon as AdrTelefoon,  
			b.Adr_Id as AdrId, 
			a.Bzk_Adr_Id as BzkAdrId, 
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
		c.Adr_Telefoon as AdrTelefoon,  
		concat(IF(b.Prs_Roepnaam="",b.Prs_Voornamen, b.Prs_Roepnaam)," ", b.Prs_Tussenvoegsels, " ", b.Prs_Achternaam) as Naam 
	from 
		oostpoort_bezoek a, oostpoort_persoon b, oostpoort_adres c
	where 
		a.Bzk_Mdw_ID=b.Prs_id and 
		a.Bzk_Adr_Id = c.Adr_Id and
		a.Bzk_Adr_Id='.$AdrId.' and ';
		
	if ($BzkId!="") {
		$query .= " a.Bzk_Id != ".$BzkId." and ";
	}
	
	$query .= '
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
		$page .= oostpoort_hiddenlink('OostpoortForm','MdwId' ,$MdwId,'VisitSearch',t('Terug'));	
	} else {
		$page .= oostpoort_hiddenlink('OostpoortForm','',0,'Cancel',t('Terug'));	
	}
		
   
   if ($readonly) {
   	// Repost selected address previous screen.
		$page.=oostpoort_keep_selection();
   }
   
	$page .= '</div>';
	$page .= '</form>';
	
   return $page;  
}

/**
 * oostpoort_visit_part
 *
 * Show all address with can be visited.
 * Visit members can selected addresses 
 * and add then to there own visit list.
 *
 * @param data			The the show data.
 * @param readonly 	The readonly flag
 *
 * @return The HTML code
 *
 * @author wplaat
 */
function oostpoort_visit_part($data, $readonly) {

	// *****************************
	// Get posted Inputs
	// *****************************
	
	$BzkId=$data->BzkId;
	
	$VisitDate=htmlentities($_POST["VisitDate"]);
	oostpoort_debug_param("VisitDate", $VisitDate);
	
	$BzkInfo=htmlentities($_POST["BzkInfo"]);
	if ($BzkInfo=="") {
		$BzkInfo=$data->BzkInfo;
	}
	oostpoort_debug_param("BzkInfo", $BzkInfo);
	
	$VisitType=htmlentities($_POST["VisitType"]);
	if ($VisitType=="") {
		$VisitType=$data->BzkBztId;
	}
	oostpoort_debug_param("VisitType", $VisitType);
	
	$NoVisit=htmlentities($_POST["NoVisit"]);
	if ($NoVisit=="") {
		if ($data->BzkBezoekNietGewenstDatum!=0) {	
			$NoVisit='on';
		}
	}
	oostpoort_debug_param("VisitType", $VisitType);
	
	if ( isset($_POST["Phone"]) ) {
		$Phone=htmlentities($_POST["Phone"]);
	} else {
	   $Phone=$data->AdrTelefoon;
	}
	oostpoort_debug_param("Phone",$Phone);

	// *****************************
	// Build page
	// *****************************
	
	$page .= '<fieldset>';
	
	$page .= '<legend><i>';
	if ($data->BzkBezoekNietGewenstDatum!=0) {
		
		$page .= t('Niet gewenst bezoek');
		if ($VisitDate=="") {
			$VisitDate=oostpoort_convert_date($data->BzkBezoekNietGewenstDatum);
		}
		
	} else if ($data->BzkAfgelegdDatum!=0) {
		$page .= t('Bezoek afgelegd');
		if ($VisitDate=="") {
			$VisitDate=oostpoort_convert_date($data->BzkAfgelegdDatum);
		}
		
	} else if ($data->BzkGeannuleerdDatum!=0) {
		$page .= t('Bezoek geannuleerd');
		if ($VisitDate=="") {
			$VisitDate=oostpoort_convert_date($data->BzkGeannuleerdDatum);
		}
	} else {
   	$page .= t('Geboekt op ').oostpoort_convert_date($data->BzkGeplandDatum); 
	}
  	$page .= '</i></legend>';
		
	$page .= oostpoort_form_item(
					t('Bezoek ID:'),							
					$data->BzkId,
					'',
					true);
					
	$page .= oostpoort_form_item(
					t('Bezoekmedewerker:'),							
					$data->Naam,
					'',
					true);
						
	$page .= oostpoort_form_item(
					t('Datum:').oostpoort_view_manitory(),							
					oostpoort_view_date("VisitDate", $VisitDate, $readonly),
					t('Datum wanneer bezoek is uitgevoerd of geannuleerd.'),
					$readonly);
						
	$page .= oostpoort_form_item(
					t('Bezoek type:').oostpoort_view_manitory(),							
					oostpoort_view_type("VisitType", $VisitType, $readonly),
					t('Selecteer wat voor type bezoek is uitgevoerd.'),
					$readonly);
						
	$page .= oostpoort_form_item(
					t('Telefoon:'),							
					oostpoort_view_text("Phone", 15, 15, $Phone, $readonly),
					t('Telefoonnummer van familie adres.'),
					$readonly);
					
	$page .= oostpoort_form_item(
					t('Notities:').oostpoort_view_manitory(),							
					oostpoort_view_note("BzkInfo", $BzkInfo, $readonly),
					t('Hier kunt u algemene niet vertrouwelijke informatie over het bezoek vastleggen.'),
					$readonly);
					
	$page .= oostpoort_form_item(
					t('Geen bezoek gewenst:'),							
					oostpoort_view_checkbox("NoVisit",$NoVisit ,$readonly),
					t('Als bezoek niet meer gewenst is selecteer deze optie. Er zullen dan geen bezoeken meer worden ingepland.'),
					$readonly);
			
	// Get Address members.
	
	if (!$readonly) {
		
		$query  = 'select ';
		$query .= '  a.Prs_Id as PrsId, ';
		$query .= '  concat(IF(a.Prs_Roepnaam="",a.Prs_Voorletters, a.Prs_Roepnaam)," ", a.Prs_Tussenvoegsels, " ", a.Prs_Achternaam) as Naam, ';
		$query .= '  a.Prs_Geboortedatum as PrsGeboorteDatum, ';
		$query .= '  a.Prs_Huwelijksdatum as PrsHuwelijksdatum, ';
		$query .= '  a.Prs_DoopDatum as PrsDoopDatum, ';
		$query .= '  a.Prs_BelijdenisDatum as PrsBelijdenisDatum '; 
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
		$query .= '  a.Prs_Geboortedatum as PrsGeboorteDatum, ';
		$query .= '  a.Prs_Huwelijksdatum as PrsHuwelijksdatum, ';
		$query .= '  a.Prs_DoopDatum as PrsDoopDatum, ';
		$query .= '  a.Prs_BelijdenisDatum as PrsBelijdenisDatum '; 
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
	$page .= '<th><b>'.t('Aanwezig').oostpoort_view_manitory().'</b></th>';
	$page .= '<th><b>'.t('Naam').'</b></th>';
	$page .= '<th class="active"><b>'.t('Geboren').'</b></th>';
	$page .= '<th><b>'.t('Huwelijk').'</b></th>';	
	$page .= '<th><b>'.t('Doop lid').'</b></th>';
	$page .= '<th><b>'.t('Belijdend lid').'</b></th>';
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
			
		$Active = $_POST["Active-".$persoon->PrsId];
		if ($Active=="") {
		
		   // Check if member is selected in database.
			$query2 = '
				select 
					Bzp_Prs_ID as BzpPrsId
				from 
					oostpoort_bezoekpersoon 
				where
					Bzp_Bzk_ID= '.$BzkId.' and
					Bzp_Prs_ID= '.$persoon->PrsId;
		
			oostpoort_debug_sql($query2);
			$queryResult2 = db_query($query2);
			$data2= db_fetch_object($queryResult2);
			if ($data2->BzpPrsId!="") {
				$Active="on";
			}
		}
	
		$page .= '<td>'.oostpoort_view_checkbox( "Active-".$persoon->PrsId, $Active, $readonly).'</td>';
		$page .= '<td>'.$persoon->Naam.'</td>';
		$page .= '<td class="active">'.oostpoort_convert_date($persoon->PrsGeboorteDatum).'</td>';
		$page .= '<td>'.oostpoort_convert_date($persoon->PrsHuwelijksdatum).'</td>';
		
		$on="";
		if ($data->PrsDoopDatum!=0) $on="on";
		$page .= '<td>'.oostpoort_view_checkbox("Doop", $on, true).'</td>';
					
		$on="";
		if ($data->PrsBelijdenisDatum!=0) $on="on";
		$page .= '<td>'.oostpoort_view_checkbox("Belijdenis", $on, true).'</td>';	
			
		$page .= '<td>'.$persoon->PrsMobiel.'</td>';
		$page .= '<td>'.$persoon->PrsEmail.'</td>';
		$page .= '</tr>';
	}
	$page .= '</tbody>';
	$page .= '</table>';

	// Button bar	
	if (!$readonly) {
		$page .= oostpoort_hiddenlink('OostpoortForm','BzkId',$data->BzkId,'VisitReady',t('Bezoek afronden')).' | ';
		$page .= oostpoort_hiddenlink('OostpoortForm','BzkId',$data->BzkId,'VisitCancel',t('Bezoek annuleren'));
	}
			
	$page .= '</fieldset>' ;
	
	return $page;
}

/**
 * oostpoort_visit_select
 *
 * Show all address with can be visited.
 * Visit members can selected addresses 
 * and add then to there own visit list.
 *
 * @return The HTML code
 *
 * @author wplaat
 */
function oostpoort_visit_select() {
  	
	// *****************************
	// Get posted Inputs
	// *****************************
	
	$MdwId=$_SESSION["MdwId"];
	oostpoort_debug_param("MdwId", $MdwId);
 	
	$BlkId=oostpoort_get_block($MdwId);
	
	// Get search value out post or session scope
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
	
	$NoVisitFilter=htmlentities($_POST['NoVisitFilter']);
	oostpoort_debug_param("NoVisitFilter", $NoVisitFilter);
	
	// *****************************
	// Build page
	// *****************************
	
	$page .= '<form name="OostpoortForm" method="POST" >';
	$page .= '<div class="oostpoort">';
	
	// Breadcrumb menu
	$node["child1"] = l(t('Bezoekwerk'),OOSTPOORT_VISIT);
	$node["child2"] = t('Bezoek toevoegen');
	oostpoort_breadcrumb($node);
	
	drupal_set_title(t('Bezoeken toevoegen' ) );
	
	$page .= '<fieldset>' ;
	$page .= '<legend>Filteren</legend>';
	
	$page .= oostpoort_form_item(
						t('Straatnaam'),
						oostpoort_view_text("Search", 40, 40, $Search, false).' '.
						oostpoort_sortbar($Sort,"VisitSearch"),
						t('Filteren op straatnaam.'),
						false);
	
	$page .= oostpoort_form_item(
						t('Bezoek niet gewenst:'),
						oostpoort_view_checkbox("NoVisitFilter", $NoVisitFilter, false),
						"Filter `bezoek niet gewenst` addressen weg",
						false);
						
	$page .= oostpoort_hiddenlink('OostpoortForm','','','VisitSearch',t('Zoek'));
	
	$page .= '</fieldset>' ;
    
	// Query Database
	$query  = '
	
	select 
    	a.Adr_id as AdrId,      
    	concat(a.Adr_Straat," ", a.Adr_Huisnr) as Straatnaam, 
    	a.Adr_Plaats as AdrPlaats, 
    	a.Adr_postcode as AdrPostcode, 
    	a.Adr_Telefoon as AdrTelefoon,  
    	a.Adr_Blk_Id as AdrBlkId,
    	(select Bzk_Id from oostpoort_bezoek where Bzk_Adr_Id=a.Adr_Id order by Bzk_Id desc limit 1) as BzkId,
 		(select Bzk_GeplandDatum from oostpoort_bezoek where Bzk_id=BzkId) as BzkGeplandDatum,
		(select Bzk_AfgelegdDatum from oostpoort_bezoek where Bzk_id=BzkId) as BzkAfgelegdDatum ,
		(select Bzk_GeannuleerdDatum from oostpoort_bezoek where Bzk_id=BzkId) as BzkGeannuleerdDatum,
		(select Bzk_BezoekNietGewenstDatum from oostpoort_bezoek where Bzk_id=BzkId) as BzkBezoekNietGewenstDatum,
		(select concat(Prs_Tussenvoegsels, " ", substring_index(Prs_Achternaam,"-",1)) from oostpoort_persoon where Prs_Huidig_Adr_Id=a.Adr_id limit 1) as Naam, 
		(select concat(IF(b.Prs_Roepnaam="",b.Prs_Voorletters, b.Prs_Roepnaam), " ", b.Prs_Tussenvoegsels, " ", b.Prs_Achternaam) from oostpoort_persoon b, oostpoort_bezoek c where c.Bzk_id=BzkId and b.prs_id=c.bzk_mdw_id) as BzkBezoekPersoon
	from 
		oostpoort_adres a  
	where ';
	if ($BlkId!=0) {
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
	$page .= '<th><b>'.t('Overige').'</b></th>';
	$page .= '</tr>';
	$page .= '</thead>';

	// Show all found address
	$page .= '<tbody>';
	$page_tmp='';
	while ($data = db_fetch_object($queryResult)) {		
	
	    // Filter out addresses without members and addresses with NoVisit flag.
		 // To hard to add this stuff in the query.
		if ((strlen($data->Naam)<2) || 
			 (($NoVisitFilter!="") && ($data->BzkBezoekNietGewenstDatum!=0))) {
			continue;
		}
		
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
		$page_tmp .= '<td class="active">'.oostpoort_hiddenlink('OostpoortForm', 'AdrId', $data->AdrId, 'VisitShow', $data->Straatnaam)."</td>";
		$page_tmp .= '<td>'.$data->AdrPostcode.'</td>';
		$page_tmp .= '<td>'.$data->AdrPlaats.'</td>';	
		$page_tmp .= '<td>'.$data->AdrBlkId.'</td>';	
		
		$page_tmp .= '<td>';
		$page_tmp .= '<div class="description">';
		
		if ($data->BzkBezoekNietGewenstDatum!=0) {
		
			$page_tmp.=t('Niet gewenst bezoek').'<br/>';
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
			
		} 
		$page_tmp .= '</div>';
		$page_tmp .= '</td>';
		
		$page_tmp .= '</tr>';
   }

	if ( $page_tmp!='') {
		$page .= $page_tmp;
	} else  {	
		$page .= '<tr class="even">';
		$page .= '<td>'.t('Geen bezoeken gevonden').'</td>';
		$page .= '</tr>';
	}
	$page .= '</tbody>';
	$page .= '</table>';

  	// Button bar
 	$page .= oostpoort_hiddenlink('OostpoortForm','',0,'VisitSelect',t('Bezoeken toevoegen')).' | ';
	$page .= oostpoort_hiddenlink('OostpoortForm','','','Cancel',t('Terug'));

	$page .= '</div>';	
	$page .= '</form>';

	return $page;  
}

// ##################################################################################
// ACTIONS
// ##################################################################################

/**
 * oostpoort_action_select
 *
 * Save selected visits
 *
 * @return The HTML code
 *
 * @author wplaat
 */
function oostpoort_action_select() {

 	// *****************************
	// Get posted Inputs
	// *****************************
	
	// Get PrsId from session scope
   $MdwId=$_SESSION["MdwId"];
	oostpoort_debug_param("MdwId", $MdwId);
	
	// *****************************
	// Update database
	// *****************************
	
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

	// *****************************
	// Create page
	// *****************************
	
   if ($amount>0) {	
		if ($mount==1) {
			$msg = t($amount.' bezoek is ingepland.');
		} else {
			$msg = t($amount.' bezoeken zijn ingepland.');
		}
		drupal_set_message($msg, 'status');
		watchdog('user', $msg);
	}
}

/**
 * oostpoort_action_select
 *
 * Delete selected visits
 *
 * @return The HTML code
 *
 * @author wplaat
 */
function oostpoort_action_deselect() {

 	// *****************************
	// Get posted Inputs
	// *****************************
	
	// Get PrsId from session scope
   $MdwId=$_SESSION["MdwId"];
	oostpoort_debug_param("MdwId", $MdwId);
	
	// *****************************
	// Update database
	// *****************************
	
	foreach( $_POST as $var => $value) {
			
		list($active, $BzkId)=split("-",$var);
		if (($active=="Active") && ($value=="on")) {
		
			oostpoort_debug_param("BzkId", $BzkId);
			
			// Check if entry all exist (Protect against reload button), if so skip insert)
			$query  = '
				delete from 
					oostpoort_bezoek 
				where 
					Bzk_id='.$BzkId;
			
			db_query($query);
			$amount++;
		}
	}

	// *****************************
	// Create page
	// *****************************
	
   if ($amount>0) {	
	
		if ($amount==1)  {
			$msg = t($amount.' bezoek is verwijderd.');
		} else { 
			$msg = t($amount.' bezoeken zijn verwijderd.');	
		}
		drupal_set_message($msg, 'status');
		watchdog('user', $msg);	
		
	} else {
	
		$msg = t('Geen bezoek geselecteerd om te verwijderen.');
		drupal_set_message($msg, 'status');

	}
}

/**
 * oostpoort_action_cancel
 *
 * Process cancel request. Validate data
 *
 * @return  1  validation error
 *          0  ok
 * 
 * @author wplaat
 */
function oostpoort_action_cancel() {	

	// *****************************
	// Get posted Inputs
	// *****************************
	
	$BzkId=htmlentities($_POST["BzkId"]);
	oostpoort_debug_param("BzkId", $BzkId);
	
	$BzkInfo=htmlentities($_POST["BzkInfo"]);
	oostpoort_debug_param("BzkInfo", $BzkInfo);
	
	$VisitType=htmlentities($_POST["VisitType"]);
	oostpoort_debug_param("VisitType", $VisitType);
	
	$CancelDate=htmlentities($_POST["VisitDate"]);
	oostpoort_debug_param("VisitDate", $VisitDate);
	
	$Phone=htmlentities($_POST["Phone"]);
	oostpoort_debug_param("Phone", $Phone);
	
	// *****************************
	// Input Validation
	// *****************************
	
	$error=0;
	
	if ($BzkInfo=="") {

		drupal_set_message(t('Annuleer notitie is verplicht'), 'error');
		$error=1;
	}

	// Validate Cancel Date
	if ($CancelDate=="") {
		drupal_set_message(t('Annuleer datum is verplicht'), 'error');
		$error=1;
		
	} else {
	
		list($day, $month, $year) = split("-",$CancelDate);
		if (!checkdate($month,$day,$year))  {
			
			drupal_set_message(t('Annuleer datum is incorrect!'), 'error');
			$error=1;
		
		} else if (mktime(0,0,0,$month,$day,$year)>mktime()) {
		
			drupal_set_message(t('Annuleer datum ligt in de toekomst!'), 'error');
			$error=1;
		}
	}
		
	if ($error==1) {
	
		return 1;
	} 
	
	// *****************************
	// Update database
	// *****************************
		
	$query  = 'UPDATE oostpoort_bezoek SET ';
	$query .= '  Bzk_GeannuleerdDatum="'.oostpoort_convert_date_reverse($CancelDate).'", ';
	$query .= '  Bzk_BezoekInfo="'.$BzkInfo.'", ';
	$query .= '  Bzk_Bzt_ID="'.$VisitType.'", ';	
	$query .= '  Bzk_AfgelegdDatum="", ';
	$query .= '  Bzk_BezoekNietGewenstDatum="" ';				
	$query .= 'where ';
	$query .= '  Bzk_ID='.$BzkId;	
	oostpoort_debug_sql($query);
	db_query($query); 

   // update phone of address
	$query  = 'select ';
	$query .= '  Bzk_Adr_ID as AdrId ';
	$query .= 'from ';
	$query .= '  oostpoort_bezoek ';
	$query .= 'where ';
	$query .= '  Bzk_ID='.$BzkId;
	oostpoort_debug_sql($query);
	$queryResult = db_query($query);
	$data = db_fetch_object($queryResult);
				
	$query  = 'UPDATE oostpoort_adres SET ';
	$query .= '  Adr_Telefoon="'.$Phone.'" ';
	$query .= 'where ';
	$query .= '  Adr_ID='.$data->AdrId;
	oostpoort_debug_sql($query);
	db_query($query);  	
	
	// *****************************
	// Create page
	// *****************************
	
	$msg .= t('Bezoek '.$BzkId.' is geannuleerd');
	drupal_set_message($msg, 'status');
	watchdog('user', $msg);
		
	return 0;
}

/**
 * oost_action_ready
 *
 * Process ready visit. Validate data. If error report it!
 *
 * @return  1  validation error
 *          0  ok
 * 
 * @author wplaat
 */
function oostpoort_action_ready()
{
	// *****************************
	// Get posted Inputs
	// *****************************
   
	$BzkId=htmlentities($_POST["BzkId"]);
	oostpoort_debug_param("BzkId", $BzkId);
	
	$NoVisit=htmlentities($_POST["NoVisit"]);
	oostpoort_debug_param("NoVisit", $NoVisit);

	$VisitType=htmlentities($_POST["VisitType"]);
	oostpoort_debug_param("VisitType", $VisitType);

	$BzkInfo=htmlentities($_POST["BzkInfo"]);
	oostpoort_debug_param("BzkInfo", $BzkInfo);
	
	$VisitDate=htmlentities($_POST["VisitDate"]);
	oostpoort_debug_param("VisitDate", $VisitDate);

	$Phone=htmlentities($_POST["Phone"]);
	oostpoort_debug_param("Phone", $Phone);
	
	// *****************************
	// Input Validation
	// *****************************
	
	$error=0;
	
	// Validate Note
	if ($BzkInfo=="") {
		drupal_set_message(t('Bezoek notitie is verplicht'), 'error');
		$error=1;
	}

    // Validate VisitDate
	if ($VisitDate=="") {
		drupal_set_message(t('Bezoek datum is verplicht'), 'error');	
		$error=1;
		
	} else {
	
		list($day, $month, $year) = split("-",$VisitDate);
		if (!checkdate($month,$day,$year))  {
		
			drupal_set_message(t('Bezoekdatum is incorrect!'), 'error');	
			$error=1;
		
		} else if (mktime(0,0,0,$month,$day,$year)>mktime()) {
		
			drupal_set_message(t('Bezoekdatum ligt in de toekomst!'), 'error');	
			$error=1;			
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
	
	if ($amount=="") {
		
		drupal_set_message(t('Geen personen geselecteerd'), 'error');	
		$error=1;	
	}
		
	if ($error==1) {
	
		return 1;
	} 

	// *****************************
	// Data ok, update database
	// *****************************
		
	$query  = 'UPDATE oostpoort_bezoek SET ';
	$query .= '  Bzk_BezoekInfo="'.$BzkInfo.'", ';
	if ($NoVisit=="on") {
	   $query .= '  Bzk_BezoekNietGewenstDatum="'.oostpoort_convert_date_reverse($VisitDate).'", ';	
	} else {
		$query .= '  Bzk_BezoekNietGewenstDatum="", ';	
		$query .= '  Bzk_AfgelegdDatum="'.oostpoort_convert_date_reverse($VisitDate).'", ';
	}
	$query .= '  Bzk_Bzt_ID="'.$VisitType.'" ';	
	$query .= 'where ';
	$query .= '  Bzk_ID='.$BzkId;
		
	oostpoort_debug_sql($query);
	db_query($query); 

	// Remove previous selection
	$query  = 'delete from oostpoort_bezoekpersoon where Bzp_Bzk_ID='.$BzkId.';';
	oostpoort_debug_sql($query);
	db_query($query);	
	
	// Add visit members
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
	
	// update phone of address
	$query  = 'select ';
	$query .= '  Bzk_Adr_ID as AdrId ';
	$query .= 'from ';
	$query .= '  oostpoort_bezoek ';
	$query .= 'where ';
	$query .= '  Bzk_ID='.$BzkId;
	oostpoort_debug_sql($query);
	$queryResult = db_query($query);
	$data = db_fetch_object($queryResult);
				
	$query  = 'UPDATE oostpoort_adres SET ';
	$query .= '  Adr_Telefoon="'.$Phone.'" ';
	$query .= 'where ';
	$query .= '  Adr_ID='.$data->AdrId;
	oostpoort_debug_sql($query);
	db_query($query);  	
   	
	// *****************************
	// Create page
	// *****************************
	
	$msg .= t('Bezoek '.$BzkId.' is afgerond');
	drupal_set_message($msg, 'status');	
	watchdog('user', $msg);	
	
	return 0;
}

// ##################################################################################
// THE END
// ##################################################################################