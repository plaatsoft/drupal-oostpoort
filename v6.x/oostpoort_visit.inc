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

  $block=$_POST['block'];
  if (strlen($block)==0) $block="0";

  // Breadcrumb menu
  $page  = '<div class="breadcrumb">';
  $page .= '<a href="'.url("").'">Home</a> > ';
  $page .= '<a href="'.url(OOSTPOORT_MENU).'">Menu</a> > ';
  $page .= 'Family > Block';
  $page .= '</div>';

  $page .= '<div class="oostpoort">';

  drupal_set_title(t('Oostpoort bezoekwerk' ) );

  $page .= '<br/>';

  $page .= '<form action="'.url(OOSTPOORT_VISIT).'" method="POST">';
  $page .= '<select name="block">';
  
  $query  = 'select concat(b.Prs_Roepnaam, " ", b.Prs_Tussenvoegsels, " ", b.Prs_Achternaam) as naam, ';
  $query .= 'b.Prs_GeslachtMV as geslacht, '; 
  $query .= 'b.Prs_Kerklidnummer as kerkid, ';
  $query .= 'b.Prs_id as PrsId, ';
  $query .= 'a.Mdw_ActiefJN as actief ';
  $query .= 'from oostpoort_medewerker a, oostpoort_persoon b ';
  $query .= 'where a.Mdw_Prs_id=b.Prs_id order by naam';
  // a.Mdw_ActiefJN LIKE "WAAR"
  $queryResult = db_query($query);
  
  while ($data = db_fetch_object($queryResult))
  {
     $page .= '<option value="'.$data->PrsId.'" ';
     if ($block==$data->PrsId) $page.='selected="selected" ';
     $page .= '>'.$data->naam.' ['.$data->PrsId.']';
	  if ($data->actief=='ONWAAR') $page .= ' (Niet Actief)';
	  $page .= '</option>';
  }
  $page .='</select> '; 
  $page .= '<input type="submit" value="'.t('Zoeken').'" />';
  $page .= '</form>';

  // Query Database
  $query  = 'select concat (c.Adr_Straat," ", c.Adr_Huisnr) as straatnaam, ';
  $query .= 'c.Adr_Plaats as plaats, ';
  $query .= 'c.Adr_postcode as postcode, ';
  $query .= 'a.Bzk_GeplandDatum as geplande, ';
  $query .= 'a.Bzk_AfgelegdDatum as afgelegd, ';
  $query .= 'a.Bzk_Adr_ID as id '; 
  $query .= 'from oostpoort_bezoek a, oostpoort_adres c ';
  $query .= 'where a.Bzk_Adr_ID=c.Adr_id and a.Bzk_Mdw_id='.$block;
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
	  $page_tmp .= '<td>'.l($data->straatnaam, OOSTPOORT_VISIT_DETAIL."/".$data->id).'</td>';
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
  $page .= '</table>';

  $query  = 'select distinct concat (d.Adr_Straat, " ", d.adr_Huisnr) as straatnaam, ';
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
  $page .= '</table>';
  
  $page .= '</div>';
  print theme("page", $page);
}


// ##################################################################################
// THE END
// ##################################################################################