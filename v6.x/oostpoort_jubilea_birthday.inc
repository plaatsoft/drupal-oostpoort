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

function oostpoort_jubilea_birthday()  
{
   $AgeId= $_POST["AgeId"];
	if ($AgeId==0) {
	   $AgeId=50;
	}
	oostpoort_debug_param("AgeId", $AgeId);

	drupal_set_title(t('Verjaardag jubilea Rapport'));
	
	// Breadcrumb menu
	$page  = '<div class="breadcrumb">';
	$page .= '<a href="'.url("").'">Start</a> > ';
	$page .= '<a href="'.url(OOSTPOORT_MENU).'">Oostpoort</a> > ';
	$page .= t('Verjaardag jubilea rapport');
	$page .= '</div>';

	$page .= '<div class="oostpoort">';
	
	$page .= '<form name="OostpoortForm" method="POST" >';
		
	$page .= '<fieldset>' ;
	$page .= '<legend>'.t('Filteren').'</legend>';
	
	$page .= '<div class="form-item">';
	$page .= '<label>'.t('Leeftijd:').' '.oostpoort_view_manitory().'</label>';
	$page .= oostpoort_age_range("AgeId", $AgeId);
	$page .= ' '.t('jaar');
	$page .= '<div class="description">'.t('Selecteer een jubilea leeftijd, daarna worden alle jubilarisen getoond.').'</div>';
	$page .= '</div>';
	
	$page .= '</fieldset>' ;

	list($year, $month, $day) = split('[/.-]', date("Y-m-d"));
	$yearStart=$year-$AgeId;
	$yearEnd=$year-$AgeId+1;	
	$startDate = $yearStart."-".$month."-".$day;
	$endDate = $yearEnd."-".$month."-".$day;
	
	 
	$query = 'select ';
	$query .= ' a.Prs_Kerklidnummer as PrsKerklidnummer, ';
	$query .= ' concat(IF(a.Prs_Roepnaam="",a.Prs_Voorletters, a.Prs_Roepnaam)," ", a.Prs_Tussenvoegsels, " ", a.Prs_Achternaam) as Naam, ';
	$query .= ' a.Prs_GeslachtMV as PrsGeslacht, ';
	$query .= ' a.Prs_EmailAdres as PrsEmail, ';
	$query .= ' a.Prs_TelefoonMobiel as PrsMobiel, ';
	$query .= ' a.Prs_Geboortedatum as PrsGeboortedatum, ';
	$query .= ' a.Prs_Huwelijksdatum as PrsHuwelijksdatum, ';
	$query .= ' concat(b.Adr_Straat, " " ,b.Adr_Huisnr) as Straat, ';
	$query .= ' b.Adr_Plaats as AdrPlaats, ';
	$query .= ' b.Adr_Postcode as AdrPostcode, ';	
	$query .= ' b.Adr_Telefoon as AdrTelefoon ';	
	$query .= 'from ';
	$query .= '  oostpoort_persoon a, oostpoort_adres b ';
	$query .= 'where ';
	$query .= ' a.Prs_Huidig_Adr_ID=b.Adr_id and ';
	if ($AgeId!=100) {
		$query .= ' a.Prs_Geboortedatum >= "'.$startDate.'" and ';
	}
	$query .= ' a.Prs_Geboortedatum <= "'.$endDate.'" and ';
	$query .= ' a.Prs_Kerklidnummer != 0 ';
	$query .= 'order by ';
	$query .= ' a.Prs_Geboortedatum';

	oostpoort_debug_sql($query);
   $queryResult = db_query($query);
   $page .= oostpoort_jubilea_birthday_table($queryResult,1);

	$page .= '<a href="'.url(OOSTPOORT_MENU).'">'.t('Terug').'</a>';
	
	$page .= '</div>';		
	$page .= '</form>';
	
	print theme("page", $page);
}

function oostpoort_jubilea_birthday_table($queryResult, $mode)
{
	$page .= '<table>';

   $page .= '<thead>';
	// Show Banner
	$page .= '<tr>';
	//$page .= '<th><b>'.t('Kerk Lidnr.').'</b></th>';
	$page .= '<th><b>'.t('Naam').'</b></th>';
	//$page .= '<th><b>'.t('Geslacht').'</b></th>';
	$page .= '<th><b>'.t('Straat').'</b></th>';
	$page .= '<th><b>'.t('Plaats').'</b></th>';
	$page .= '<th><b>'.t('Postcode').'</b></th>';
	$page .= '<th class="active"><b>'.t('Geboren').'</b></th>';
   //$page .= '<th><b>'.t('Huwelijk datum').'</b></th>';
	//$page .= '<th><b>'.t('Telefoon').'</b></th>';
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
      //$page_tmp .= '<td>'.$data->PrsKerklidnummer.'</td>';
      $page_tmp .= '<td>'.$data->Naam.'</td>';
		//$page_tmp .= '<td>'.oostpoort_view_geslacht($data->PrsGeslacht).'</td>';
		$page_tmp .= '<td>'.$data->Straat.'</td>';
		$page_tmp .= '<td>'.$data->AdrPlaats.'</td>';  
		$page_tmp .= '<td>'.$data->AdrPostcode.'</td>';
		$page_tmp .= '<td class="active">'.oostpoort_view_date($data->PrsGeboortedatum).'</td>';
		//$page_tmp .= '<td>'.oostpoort_view_date($data->PrsHuwelijksdatum).'</td>';
		//$page_tmp .= '<td>'.$data->AdrTelefoon.'</td>';
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


function oostpoort_age_range($id, $AgeId) {

	$page.='<select name="'.$id.'" onChange="submit();" >';
	
	$x=array(50, 65, 70, 75, 80, 85, 90, 95, 100);
		
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