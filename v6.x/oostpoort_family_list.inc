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
// FAMILY LIST VIEW
// ##################################################################################

function oostpoort_family_list()  
{
	$action=$_POST['action'];
	echo "[action=".$action."]";
        	  
	if ($action==" List") {
  
     $page .= oostpoort_address_list();
	
	} else  {
	
	  $page .= oostpoort_address_list();
	
	}
	print theme("page", $page);
}
	
/*
 * Function shows all family in a list form
 * @return HTML
 */
function oostpoort_address_list() {

	$page .= '<div class="oostpoort">';

	// Get search value out post or session scope
	$search=$_POST['search'];
	echo "[search=".$search."]";
	
	// Get sort value 
	$Sort=$_POST['Sort'];
	echo "[Sort=".$Sort."]";
		
	// Breadcrumb menu
	$page  = '<div class="breadcrumb">';
	$page .= '<a href="'.url("").'">Start</a> > ';
	$page .= '<a href="'.url(OOSTPOORT_MENU).'">Oostpoort</a> > ';
	$page .= t('Adressen Lijst');
	$page .= '</div>';

	$page .= '<div class="oostpoort">';
	
	drupal_set_title(t('Oostpoort Adressen lijst'));

	$page .= '<br/>';

	$page .= '<form name="OostpoortForm" method="POST" >';
	
	$page .= '<input id="text" name="search" size="40" maxlength="40" type="text" value="'.$search.'" ';
	$page .= '<input type="submit" value="'.t('Zoeken').'" />';

	$sort_bar=array("A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P", "Q","R","S","T","U","V","W","X","Y","Z");
	if ($sort=='') $page .= '<B>';
	$page .= oostpoort_hiddenlink('OostpoortForm', 'Sort', '', '', t('ALLES')).'|';
	if ($sort=='') $page .= '</B>';
	for ($i=0; $i<26; $i++)
	{
		if ($sort==$sort_bar[$i]) $page .= '<B>';
		$page .= oostpoort_hiddenlink('OostpoortForm', 'Sort', $sort_bar[$i], '', $sort_bar[$i]).'|';
		if ($sort==$sort_bar[$i]) $page .= '</B>';
	}
  
  
  	$query = 'select ';
	$query .= ' a.Prs_Kerklidnummer as PrsKerklidnummer, ';
	$query .= ' concat(IF(a.Prs_Roepnaam="",a.Prs_Voorletters, a.Prs_Roepnaam)," ", a.Prs_Tussenvoegsels, " ", a.Prs_Achternaam) as Naam, ';
	$query .= ' a.Prs_GeslachtMV as PrsGeslacht, ';
	$query .= ' a.Prs_EmailAdres as PrsEmail, ';
	$query .= ' a.Prs_TelefoonMobiel as PrsMobiel, ';
	$query .= ' a.Prs_Geboortedatum as PrsGeboortedatum, ';
	$query .= ' concat(b.Adr_Straat, " " ,b.Adr_Huisnr) as Straat, ';
	$query .= ' b.Adr_id as AdrId, ';
	$query .= ' b.Adr_Plaats as AdrPlaats, ';
	$query .= ' b.Adr_Postcode as AdrPostcode, ';	
	$query .= ' b.Adr_Telefoon as AdrTelefoon ';	
	$query .= 'from ';
	$query .= '  oostpoort_persoon a, oostpoort_adres b ';
	$query .= 'where ';
	$query .= ' a.Prs_Huidig_Adr_ID=b.Adr_id and ';
	if (strlen($search)==0) {
		$query  .= 'a.Prs_Achternaam LIKE "'.$Sort.'%" ';
	} else { 
		$query  .= 'a.Prs_Achternaam LIKE "'.$search.'%" ';
   }
	$query .= 'order by ';
	$query .= ' a.Prs_Achternaam';
	
  
	$queryResult = db_query($query);

	$page .= '<table>';
	$page .= '<thead>';
	$page .= '<tr>';
	$page .= '<td><b>'.t('Naam').'</b></th>';
	$page .= '<th><b>'.t('Straat').'</b></th>';
	$page .= '<th><b>'.t('Postcode').'</b></th>';
	$page .= '<th><b>'.t('Plaats').'</b></th>';
	$page .= '</tr>';
   $page .= '</thead>';
 
	$page .= '<tbody>';
	$page_tmp='';
	while ($data = db_fetch_object($queryResult))
	{
		if ((++$i%2)==0) {
			$page_tmp .= '<tr class="even">';
		} else {
			$page_tmp .= '<tr class="odd">';
		}
		$page_tmp .= '<td>'.oostpoort_hiddenlink('OostpoortForm', 'AdrId', $data->AdrId, '', $data->Naam)."</td>";
		$page_tmp .= '<td>'.$data->Straat.'</td>';
		$page_tmp .= '<td>'.$data->AdrPostcode.'</td>';
		$page_tmp .= '<td>'.$data->AdrPlaats.'</td>';
		$page_tmp .= '</tr>';
	}

	if ( $page_tmp!='') {
		// Show Content
		$page .= $page_tmp;
	} else  {
		// No content found
		$page .= '<tr>';
		$page .= '<td>'.t('Geen informatie gevonden').'</td>';
		$page .= '<td></td>';
		$page .= '<td></td>';
		$page .= '<td></td>';
		$page .= '<td>';
		$page .= '</tr>';
	}
	$page .= '</tbody>';
	$page .= '</table>';

   return $page;
}

// ##################################################################################
// THE END
// ##################################################################################