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

function oostpoort_jubilea_married() {

	//echo var_dump($_POST);
	
	// *****************************
	// Check access for this page.
	// *****************************
	
	if (!oostpoort_visit_member_access()) {
		return drupal_access_denied();
	}
	
	$Action=$_POST['Action'];
	oostpoort_debug_param("Action", $Action);
	        	  
	if ($Action=="FamilyView") {
     	$page = oostpoort_family_view(true);     	
     	$page .= '<a href="'.url(OOSTPOORT_JUBILEA_MARRIED).'">'.t('Terug').'</a>';     	
	} else {
		$page = oostpoort_jubilea_married_list();
	}
	print theme("page", $page);
}

// ##################################################################################
// Views
// ##################################################################################

function oostpoort_jubilea_married_list() {

	// *****************************
	// Create page
	// *****************************
	
	// Build page
	$page .= '<form name="OostpoortForm" method="POST" >';
	$page .= '<div class="oostpoort">';
		
	// Breadcrumb menu
	$node["child1"] = '<a href="'.url(OOSTPOORT_JUBILEA_MARRIED).'">'.t('Huwelijksjubilaris Rapport').'</a>';
	$page .= oostpoort_breadcrumb($node);

	// title
	drupal_set_title(t('Huwelijksjubilaris Rapport' ) );

	$page .= '<fieldset>' ;
	$page .= '<b>'.t('Rapport loopt over de volgende periode:').'</b>'.'<br/>';
	$page .= t('2 maanden terug t/m 4 maanden vooruit.');
	$page .= '</fieldset>' ;
	
	$x=array(12, 25, 40, 45, 50, 55);
		
	foreach ($x as $AgeId) {
		
		$page .= '<fieldset>' ;
		$page .= '<legend>';
		$page .= t('Dit zijn de huwelijksjubilarrisen van').' ';
		if ($AgeId<55) {
			$page .= $AgeId.' '.t('jaar.');
		} else {
			$page .= $AgeId.' '.t('jaar en ouder.');
		}
		$page .= '</legend>';
	
		$periode_start=oostpoort_offset_date( -2*31 );
		list($year, $month, $day) = split('-', $periode_start);
		$yearStart=$year-$AgeId;
		$startDate = $yearStart."-".$month."-".$day;
	
		$periode_end=oostpoort_offset_date( 4*31 );
		list($year, $month, $day) = split('-', $periode_end);
		$yearEnd=$year-$AgeId;
		$endDate = $yearEnd."-".$month."-".$day;
			
		$query = '
			select 
				concat("Familie ", a.Prs_Tussenvoegsels, " ", substring_index(a.Prs_Achternaam,"-",1)) as Naam, 
				a.Prs_Huwelijksdatum as PrsHuwelijksdatum, 
				concat(b.Adr_Straat, " " ,b.Adr_Huisnr) as Straat, 
				b.Adr_Plaats as AdrPlaats, 
				b.Adr_Postcode as AdrPostcode, 
				b.Adr_ID as AdrId 
			from 
				oostpoort_persoon a, 
				oostpoort_adres b 
			where 
				a.Prs_Huidig_Adr_ID=b.Adr_id and 
				a.Prs_Huwelijksdatum!=0 and ';
		
		if ($AgeId<55) {
			$query .= ' a.Prs_Huwelijksdatum >= "'.$startDate.'" ';
			$query .= ' and a.Prs_Huwelijksdatum <= "'.$endDate.'" ';
		} else {
			$query .= ' a.Prs_Huwelijksdatum <= "'.$startDate.'" ';
		}
			
		$query .= 'group by 
				Straat, a.Prs_Huwelijksdatum having ( COUNT(Prs_Huwelijksdatum)=2) 
			order by
				a.Prs_Huwelijksdatum ';
	
		oostpoort_debug_sql($query);
		$queryResult = db_query($query);
		$page .= oostpoort_jubilea_married_table($queryResult,1);
		
		$page .= '</fieldset>';
	}
	
	$page .= '<a href="'.url(OOSTPOORT_MENU).'">'.t('Terug').'</a>';
	
	$page .= '</div>';
	$page .= '</form>';
	
	return $page;
}

function oostpoort_jubilea_married_table($queryResult, $mode) {

	$page .= '<table>';

	$page .= '<thead>';
	$page .= '<tr>';
	$page .= '<th><b>'.t('Naam').'</b></th>';
	$page .= '<th><b>'.t('Straat').'</b></th>';
	$page .= '<th><b>'.t('Plaats').'</b></th>';
	$page .= '<th><b>'.t('Postcode').'</b></th>';
  	$page .= '<th class="active"><b>'.t('Huwelijksdatum').'</b></th>';
	$page .= '</tr>';
	$page .= '</thead>';
	$page .= '<tbody>';
		
	// Show all found members
	$page_tmp='';
	while ($data = db_fetch_object($queryResult))
	{
		if ((++$i%2)==0) {
			$page_tmp .= '<tr class="even">';
		} else {
			$page_tmp .= '<tr class="odd">';
		}
		
		$page_tmp .= '<td>';
		$page_tmp .= oostpoort_hiddenlink('OostpoortForm', 'AdrId', $data->AdrId, 'FamilyView', $data->Naam);
		$page_tmp .= '</td>';
		
		$page_tmp .= '<td>'.$data->Straat.'</td>';
		$page_tmp .= '<td>'.$data->AdrPlaats.'</td>';  
		$page_tmp .= '<td>'.$data->AdrPostcode.'</td>';
		$page_tmp .= '<td class="active">'.oostpoort_convert_date($data->PrsHuwelijksdatum).'</td>';
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

	return $page;
}

// ##################################################################################
// THE END
// ##################################################################################



