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
	// Get posted Inputs
	// *****************************
	
   $AgeId= $_POST["AgeId"];
	if ($AgeId=="") {
		$AgeId=$_SESSION["AgeId"];
		if ($AgeId=="") {
	   		$AgeId=12;
		}
	}
	$_SESSION["AgeId"]=$AgeId;
	oostpoort_debug_param("AgeId", $AgeId);

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
	$page .= '<legend>'.t('Filteren').'</legend>';
	
	$page .= '<div class="form-item">';
	$page .= '<label>'.t('Huwelijk:').' '.oostpoort_view_manitory().'</label>';
	$page .= oostpoort_married_range("AgeId", $AgeId).' '.t('jaar');
	$page .= '<div class="description">'.t('Selecteer een huwelijk lusterum, daarna worden alle jubilarisen getoond.').'</div>';
	$page .= '</div>';
	
	$page .= '</fieldset>' ;

	list($year, $month, $day) = split('[/.-]', date("Y-m-d"));
	$yearStart=$year-$AgeId;
	$yearEnd=$year-$AgeId+1;	
	$startDate = $yearStart."-".$month."-".$day;
	$endDate = $yearEnd."-".$month."-".$day;
	
	$query = 'select ';
	$query .= ' concat("Familie ", a.Prs_Tussenvoegsels, " ", substring_index(a.Prs_Achternaam,"-",1)) as Naam, ';
    $query .= ' a.Prs_GeslachtMV as PrsGeslacht, ';
	$query .= ' a.Prs_Huwelijksdatum as PrsHuwelijksdatum, ';
	$query .= ' concat(b.Adr_Straat, " " ,b.Adr_Huisnr) as Straat, ';
	$query .= ' b.Adr_Plaats as AdrPlaats, ';
	$query .= ' b.Adr_Postcode as AdrPostcode, ';
	$query .= ' b.Adr_ID as AdrId ';
	$query .= 'from ';
	$query .= '  oostpoort_persoon a, oostpoort_adres b ';
	$query .= 'where ';
	$query .= ' a.Prs_Huidig_Adr_ID=b.Adr_id and ';
	$query .= ' a.Prs_Huwelijksdatum >= "'.$startDate.'" and ';
	$query .= ' a.Prs_Huwelijksdatum <= "'.$endDate.'" ';
	$query .= 'group by ';
	$query .= '	Straat, Prs_Huwelijksdatum having ( COUNT(Prs_Huwelijksdatum)=2) '; 
	$query .= 'order by ';
	$query .= ' a.Prs_Huwelijksdatum';
	
   oostpoort_debug_sql($query);
   $queryResult = db_query($query);
   $page .= oostpoort_jubilea_married_table($queryResult,1);

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

function oostpoort_married_range($id, $AgeId) {

	$page.='<select name="'.$id.'" onChange="submit();" >';
	
	$x=array(12, 25, 40, 45, 50, 55, 60, 65, 70);
		
	foreach ($x as $value)
	{
		$page.='<option value="'.$value.'" ';
		if ($value==$AgeId) {
			$page.='selected="selected" ';
		}
		$page.='>'.$value.'</option>';
	}
	$page.='</select>';
	  
   return $page;
}

// ##################################################################################
// THE END
// ##################################################################################



