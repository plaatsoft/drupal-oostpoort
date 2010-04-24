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
// FAMILY LIST BLOCK
// ##################################################################################

/**
 * Render a page showing the visit list
 * @return HTML
 */
function oostpoort_visit()  
{
  global $user;

  $action=$_POST['action'];
  echo "[action=".$action."]";
  
  $prsId=$_POST["PrsId"];
  echo "[prsId=".$prsId."]";
  
     // Breadcrumb menu
  $page  = '<div class="breadcrumb">';
  $page .= '<a href="'.url("").'">Start</a> > ';
  $page .= 'Oostpoort > Visit';
  $page .= '</div>';
  
  if ($action=="Select") {
  	  
  } else {
  
	  $page .= oostpoort_visit_list_view($prsId);
  }
  
  print theme("page", $page);
}


function oostpoort_visit_list_view($PrsId) {
  
  // Breadcrumb menu
  $page  = '<div class="breadcrumb">';
  $page .= '<a href="'.url("").'">Home</a> > ';
  $page .= '<a href="'.url(OOSTPOORT_MENU).'">Menu</a> > ';
  $page .= 'Family > Block';
  $page .= '</div>';

  $page .= '<div class="oostpoort">';

  drupal_set_title(t('Oostpoort bezoekwerk' ) );

  $page .= '<br/>';
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
  $page .= oostpoort_hiddenlink('OostpoortForm','',0,'Search',t('Zoeken'));
  
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
  echo $query;
  
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
		$page_tmp .= '<td>'.oostpoort_hiddenlink('OostpoortForm', 'AdrId', $data->AdrId, 'Select', $data->Straatnaam).'</td>';
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


// ##################################################################################
// THE END
// ##################################################################################