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

/*
 * Function shows all family in a list form
 * @return HTML
 */
function oostpoort_family_list( $sort='', $search='') {

  global $user;

  // Breadcrumb menu
  $page  = '<div class="breadcrumb">';
  $page .= '<a href="'.url("").'">Home</a> > ';
  $page .= '<a href="'.url(OOSTPOORT_MENU).'">Menu</a> > ';
  $page .= 'Family';
  $page .= '</div>';

  $page .= '<div class="oostpoort">';

  drupal_set_title(t('Oostpoort familie lijst'));

  // Get search value out post or session scope
  $search=$_POST['search'];

  // Set sort value in session scope
  $_SESSION["sort"]=$sort;

  $page .= '<br/>';
  $page .= '<table border=0 >';
  $page .= '<tr>';

  $page .= '<td >';
  $page .= '<form action="'.url(OOSTPOORT_FAMILY_LIST).'" method="POST">';
  $page .= '<input id="text" name="search" size="40" maxlength="40" type="text" value="'.$search.'" ';
  $page .= '<input type="submit" value="'.t('Zoeken').'" />';
  $page .= '</form>';
  $page .= '</td>';

  //$page .= '<table border=0 >';
  $page .= '<tr>';

  $page .= '<td ALIGN="left">';
  $sort_bar=array("A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P", "Q","R","S","T","U","V","W","X","Y","Z");
  if ($sort=='') $page .= '<B>';
  $page .= '<a href="'.url(OOSTPOORT_FAMILY_LIST).'">'.t('ALLES').'</a>|';
  if ($sort=='') $page .= '</B>';
  for ($i=0; $i<26; $i++)
  {
     if ($sort==$sort_bar[$i]) $page .= '<B>';
     $page .= '<a href="'.url(OOSTPOORT_FAMILY_LIST.'/'.$sort_bar[$i]).'">'.$sort_bar[$i].'</a>|';
     if ($sort==$sort_bar[$i]) $page .= '</B>';
  }
  $page .= '</td>';

  $page .= '</tr>';
  $page .= '</table>';

  // Query Database
  $query  = 'select distinct achternaam, straatnaam, huisnummer, postcode, woonplaats from {oostpoort_members} where ';
  if (strlen($search)==0)
  {
    $query  .= 'achternaam LIKE "'.$sort.'%" order by achternaam';
  }
  else
  { 
    $query  .= 'achternaam LIKE "'.$search.'%" order by achternaam';
  }
  $queryResult = db_query($query);

  $page .= '<table width=100% border="1" cellpadding="3" cellspacing="3">';

  // Show Banner
  $page .= '<td>';
  $page .= '<b>'.t('Naam').'</b>';
  $page .= '</td><td>';
  $page .= '<b>'.t('Straat').'</b>';
  $page .= '</td><td>';
  $page .= '<b>'.t('Postcode').'</b>';
  $page .= '</td><td>';
  $page .= '<b>'.t('Plaats').'</b>';
  $page .= '</td></tr>';

  // Show all found address
  $page_tmp='';
  while ($data = db_fetch_object($queryResult))
  {
    //$key=oostpoort_encode($data->postcode.$data->huisnummer);
    $key=$data->postcode.$data->huisnummer;
    $page_tmp .= '<tr><td width=60>';
    $page_tmp .= l($data->achternaam, OOSTPOORT_FAMILY_VIEW."/".$key);
    $page_tmp .= '</td><td>';
    $page_tmp .= $data->straatnaam.' '.$data->huisnummer;
    $page_tmp .= '</td><td>';
    $page_tmp .= $data->postcode;
    $page_tmp .= '</td><td>';
    $page_tmp .= $data->woonplaats;
    $page_tmp .= '</td></tr>';
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

  print theme("page", $page);
}

// ##################################################################################
// THE END
// ##################################################################################