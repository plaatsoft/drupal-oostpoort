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

function oostpoort_member_age() {
	
	//echo var_dump($_POST);
	
	// *****************************
	// Check access for this page.
	// *****************************
	
	if (!oostpoort_visit_member_access()) {
		return drupal_access_denied();
	}
	
	$Action=$_POST['Action'];
	oostpoort_debug_param("Action", $Action);
	
	switch ($Action) {
   
		case 'MemberView':
			// Create Page
     		$page = oostpoort_member_view(true);
     		$page .= '<a href="'.url(OOSTPOORT_MEMBER_AGE).'">'.t('Terug').'</a>';

     		// Overwrite default breadcrumb menu
			$node["child1"] = '<a href="'.url(OOSTPOORT_MEMBER_AGE).'">'.t('Leeftijdscategorie Rapport').'</a>';
			$node["child2"] = t('Lid Detail');
			oostpoort_breadcrumb($node);
			break;
			
		default: 
			$page = oostpoort_member_age_list();
			break;
	}
	
	print theme("page", $page);
}

// ##################################################################################
// Views
// ##################################################################################

function oostpoort_member_age_list() {

	// *****************************
	// Get posted Inputs
	// *****************************
	
	$StartDate=$_POST["StartDate"];
	if ($StartDate=="") {
		$StartDate=$_SESSION['StartDate']; 
		if ($StartDate=="") {
			$StartDate=date("d-m-Y");	
		}
	} 
	$_SESSION['StartDate']=$StartDate; 
	oostpoort_debug_param("StartDate", $StartDate);
	
	$EndDate= $_POST["EndDate"];
	if ($EndDate=="") {
		$EndDate=$_SESSION['EndDate'];
		if ($EndDate=="") { 
	   		$EndDate=date("d-m-Y");
		}
	}
	$_SESSION['EndDate']=$EndDate; 
	oostpoort_debug_param("EndDate", $EndDate);

	// *****************************
	// Query
	// *****************************

	$query  = 'select ';
	$query .= ' a.Prs_Id as PrsId, ';
	$query .= ' a.Prs_Kerklidnummer as PrsKerklidnummer, ';
	$query .= ' concat(IF(a.Prs_Roepnaam="",a.Prs_Voorletters, a.Prs_Roepnaam)," ", a.Prs_Tussenvoegsels, " ", a.Prs_Achternaam) as Naam, ';
	$query .= ' a.Prs_GeslachtMV as PrsGeslacht, ';
	$query .= ' a.Prs_EmailAdres as PrsEmail, ';
	$query .= ' a.Prs_TelefoonMobiel as PrsMobiel, ';
	$query .= ' a.Prs_Geboortedatum as PrsGeboortedatum, ';
	$query .= ' concat(b.Adr_Straat, " " ,b.Adr_Huisnr) as Straat, ';
	$query .= ' b.Adr_Plaats as AdrPlaats, ';
	$query .= ' b.Adr_Postcode as AdrPostcode, ';	
	$query .= ' b.Adr_Telefoon as AdrTelefoon ';	
	$query .= 'from ';
	$query .= '  oostpoort_persoon a, oostpoort_adres b ';
	$query .= 'where ';
	$query .= ' a.Prs_Huidig_Adr_ID=b.Adr_id and ';
	$query .= ' a.Prs_Geboortedatum >= "'.oostpoort_convert_date_reverse($StartDate).'" and ';
	$query .= ' a.Prs_Geboortedatum <= "'.oostpoort_convert_date_reverse($EndDate).'" ';
	$query .= 'order by ';
	$query .= ' a.Prs_Geboortedatum desc';

	oostpoort_debug_sql($query);
   	$queryResult = db_query($query);
   	
	// *****************************
	// Create page
	// *****************************

	$page .= '<form name="OostpoortForm" method="POST" >';
	$page .= '<div class="oostpoort">';
	
	// Breadcrumb menu
	$node["child1"] = '<a href="'.url(OOSTPOORT_MEMBER_AGE).'">'.t('Leeftijdscategorie Rapport').'</a>';
	oostpoort_breadcrumb($node);

	// title
	drupal_set_title(t('Leeftijdscategorie Rapport' ));
			
	$page .= '<fieldset>' ;
	$page .= '<legend>'.t('Filteren').'</legend>';
	
	$page .= '<div class="form-item">';
	$page .= '<label>'.t('Start datum:').' '.oostpoort_view_manitory().'</label>';
	$page .= '<input type="text" name="StartDate" size="11" maxlength="10" value="'.$StartDate.'"/>';
	$page .= '<div class="description">'.t('Voer een geboorte datum in. Formaat (dd-mm-jjjj)').'</div>';
	$page .= '</div>';
	
	$page .= '<div class="form-item">';
	$page .= '<label>'.t('Eind datum:').' '.oostpoort_view_manitory().'</label>';
	$page .= '<input type="text" name="EndDate" size="11" maxlength="10" value="'.$EndDate.'"/>  '; 
	$page .= '<div class="description">'.t('Voer een geboorte datum in. Formaat (dd-mm-jjjj)').'</div>';
	$page .= '</div>';
	
	$page .= oostpoort_hiddenlink('OostpoortForm','','','Search',t('Zoeken'));
	
	$page .= '</fieldset>';

	$page .= oostpoort_member_age_table($queryResult,1);
	   	
	$page .= '<a href="'.url(OOSTPOORT_MENU).'">'.t('Terug').'</a>';
	
	$page .= '</div>';
	$page .= '</form>';
		
	return $page;
}

function oostpoort_member_age_table($queryResult, $mode) {

	$page .= '<table>';

	$page .= '<thead>';
	$page .= '<tr>';
	$page .= '<th><b>'.t('KerkLidNr').'</b></th>';
	$page .= '<th><b>'.t('Naam').'</b></th>';
	$page .= '<th><b>'.t('Geslacht').'</b></th>';
	$page .= '<th><b>'.t('Straat').'</b></th>';
	$page .= '<th><b>'.t('Plaats').'</b></th>';
	$page .= '<th><b>'.t('Postcode').'</b></th>';
	$page .= '<th class="active"><b>'.t('Geboren').'</b></th>';
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
     	$page_tmp .= $data->PrsKerklidnummer;
     	$page_tmp .= '</td>';
     			
     	$page_tmp .= '<td>';
     	$page_tmp .= oostpoort_hiddenlink('OostpoortForm', 'PrsId', $data->PrsId, 'MemberView', $data->Naam);
     	$page_tmp .= '</td>';
     	
		$page_tmp .= '<td>';
		$page_tmp .= oostpoort_view_geslacht($data->PrsGeslacht);
		$page_tmp .= '</td>';
		
		$page_tmp .= '<td>'.$data->Straat.'</td>';
		$page_tmp .= '<td>'.$data->AdrPlaats.'</td>';  
		$page_tmp .= '<td>'.$data->AdrPostcode.'</td>';
		
		$page_tmp .= '<td class="active">';
		$page_tmp .= oostpoort_convert_date($data->PrsGeboortedatum);
		$page_tmp .= '</td>';
		
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
