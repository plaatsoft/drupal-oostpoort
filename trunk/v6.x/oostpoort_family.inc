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

function oostpoort_family()  
{
	$action=$_POST['action'];
	echo "[Action=".$action."]";
        	  
	if ($action=="List") {
  
     $page .= oostpoort_family_list();
	
	} else  {
	
	  $page .= oostpoort_family_list();
	
	}
	print theme("page", $page);
}
	
/*
 * Function shows all family in a list form
 */
function oostpoort_family_list() {

	$page .= '<div class="oostpoort">';

	// Get search value out post or session scope
	$Search=$_POST['Search'];
	echo "[Search=".$Search."]";
	
	// Get sort value 
	$Sort=$_POST['Sort'];
	if ($Sort=="") {
	   $Sort="A";
	}
	echo "[Sort=".$Sort."]";
		
	// Breadcrumb menu
	$page  = '<div class="breadcrumb">';
	$page .= '<a href="'.url("").'">Start</a> > ';
	$page .= '<a href="'.url(OOSTPOORT_MENU).'">Oostpoort</a> > ';
	$page .= t('Adressen Lijst');
	$page .= '</div>';

	$page .= '<div class="oostpoort">';
	
	drupal_set_title(t('Oostpoort Familie overzicht'));

	$page .= '<br/>';

	$page .= '<form name="OostpoortForm" method="POST" >';
	
	$page .= '<input id="text" name="Search" size="40" maxlength="40" type="text" value="'.$search.'" ';
	$page .= '<input type="submit" value="'.t('Zoeken').'" />';

	$page .= '<br/>';
	
	$page .= oostpoort_sortbar($Sort);
	  
  	$query = 'select ';
	$query .= ' distinct b.Adr_id as AdrId, ';
	//$query .= ' concat(a.Prs_Tussenvoegsels, " ", a.Prs_Achternaam) as Naam, ';
	$query .= ' concat(b.Adr_Straat, " " ,b.Adr_Huisnr) as Straat, ';
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


function oostpoort_sortbar($sort)
{
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
	return $page;
}
	
// ##################################################################################
// THE END
// ##################################################################################