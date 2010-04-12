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
// FAMILY MEMBER VIEW
// ##################################################################################

/**
 * Render a page showing the selected family in detail
 * @return HTML
 */
function oostpoort_family_view( $parameters)  
{
  global $user;

  // Breadcrumb menu
  $page  = '<div class="breadcrumb">';
  $page .= '<a href="'.url("").'">Home</a> > ';
  $page .= '<a href="'.url(OOSTPOORT_MENU).'">Menu</a> > ';
  $page .= '<a href="'.url(OOSTPOORT_FAMILY_LIST).'/'.$_SESSION["sort"].'">Family</a> > ';
  $page .= 'View';
  $page .= '</div>';

  $page .= '<div class="oostpoort">';

  // Set sort value in session scope
  $_SESSION["parameters"]=$parameters;

  drupal_set_title(t('Oostpoort familie overzicht') );

  // Debugging value
  $border=0;

  //$parameters=oostpoort_decode($parameters);
  //if ($parameters==false) 
  //{ 
  //   print theme("page", $page);
  //   return "";
  // }

  $zipcode=substr($parameters,0,7);
  $nr=substr($parameters,7);

  $query  = 'SELECT mid, geslacht, aanschrijf_naam, achternaam, geboorte_datum, straatnaam, huisnummer, postcode, ';
  $query .= 'woonplaats, kerklidnr FROM {oostpoort_members} WHERE postcode="'.$zipcode.'" AND huisnummer="'.$nr.'" order by geboorte_datum';
  $queryResult = db_query($query);
  $data = db_fetch_object($queryResult);

  $telephone=oostpoort_get_item2($data->kerklidnr,2);
  $street=$data->straatnaam.' '.$data->huisnummer;
  $city=$data->woonplaats;

  // Show address information in readonly mode
  $page .= '<br/>';

  $page .= '<table width="100%" cellpadding="1" >';

  $page .= '<tr><td valign="top">';
  $page .= '<b>'.$data->achternaam.'</b><br/>';
  $page .= $street.'<br/>';
  $page .= $data->postcode.'&nbsp;&nbsp;'.$data->street.'&nbsp;&nbsp;'.$city.'<br/>';
  $page .= 'Nederland<br/>';
  $page .= $telephone.'<br/>';
  $page .= '</td>';

  $page .= '</tr>';
  $page .= '</table>';

  $page .= '<table width=100% cellpadding=1>';

  // Show Banner
  $page .= '<tr><td>';
  if (user_access('access administration pages')) 
  {
    $page .= '<b>'.t('Id').'</b>';
    $page .= '</td><td>';
  }
  $page .= '<b>'.t('Foto').'</b>';
  $page .= '</td><td>';
  $page .= '<b>'.t('Geslacht').'</b>';
  $page .= '</td><td>';
  $page .= '<b>'.t('Naam').'</b>';
  $page .= '</td><td>';
  $page .= '<b>'.t('Geboren').'</b>';
  $page .= '</td><td>';
  $page .= '<b>'.t('Mobiel').'</b>';
  $page .= '</td><td>';
  $page .= '<b>'.t('Email').'</b>';
  $page .= '</td></tr>';

  // Show all found members
  do
  {
    $email=oostpoort_get_item2($data->kerklidnr,1);
    $mobile=oostpoort_get_item2($data->kerklidnr,3);
    $image=oostpoort_get_item2($data->kerklidnr,8);

    $page .= '<tr><td>';
    if (user_access('access administration pages')) 
    {
       $page .= $data->kerklidnr;
       $page .= '</td><td>';
    }
    if (strlen($image)>0)
    {
      $page .= '<a href="'.url(OOSTPOORT_MEMBER_VIEW.'/'.$data->mid).'">';
      $filename=url(OOSTPOORT_THUMBNAILS_DIR.'/'.$image);
      $filename=str_replace(array("?q="), "", $filename);
      $page .= '<img src="'.$filename.'" width=25 height=30/></a>';
    }
    $page .= '</td><td>';
    $page .= $data->geslacht;
    $page .= '</td><td>';
    $page .= l($data->aanschrijf_naam,OOSTPOORT_MEMBER_VIEW.'/'.$data->mid);
    $page .= '</td><td>';
    list($year, $month, $day) = split('[/.-]', $data->geboorte_datum);
    $page .= $day.'-'.$month.'-'.$year;
    $page .= '</td><td>';
    $page .= $mobile;
    $page .= '</td><td>';
    $page .= l($email,'mailto:'.$email);
    $page .= '</td></tr>';
  }
  while ($data = db_fetch_object($queryResult));

  $page .= '</table>';
  $page .= '<br/>';

  // Navigation Bar
  $page .= '<table border="0" >';
  $page .= '<tr>';

  $page .= '<td align="left">';
  $page .= '<form action="'.url(OOSTPOORT_FAMILY_LIST.'/'.$_SESSION["sort"]).'" method="POST">';
  $page .= '<input type="submit" value="'.t('Terug').'" />';
  $page .= '</form>';
  $page .= '</td>';

  $page .= '<td align="left">';
  $page .= '<form action="'.url(OOSTPOORT_FAMILY_MAP).'" method="POST">';
  $page .= '<input type="hidden" name="street" value="'.$street.'" />';
  $page .= '<input type="hidden" name="city" value="'.$city.'" />';
  $page .= '<input type="hidden" name="country" value="Nederland" />';
  $page .= '<input type="submit" value="'.t('Kaart').'" />';
  $page .= '</form>';
  $page .= '</td>';

  $page .= '</tr>';
  $page .= '</table>';

  $page .= '</div>';
  print theme("page", $page);
}

// ##################################################################################
// THE END
// ##################################################################################
