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
// MEMBER LIST AGE
// ##################################################################################

/**
 * Render a page showing the selected address for editing
 * @return HTML
 */
function oostpoort_member_age()  
{
  global $user;

  $age=$_POST['age'];
  if (strlen($age)==0) $age=0;

  // Breadcrumb menu
  $page  = '<div class="breadcrumb">';
  $page .= '<a href="'.url("").'">Home</a> > ';
  $page .= '<a href="'.url(OOSTPOORT_MENU).'">Menu</a> > ';
  $page .= 'Members > Age';
  $page .= '</div>';

  $page .= '<div class="oostpoort">';

  drupal_set_title(t('Oostpoort ledenlijst gefilterd op leeftijd' ) );

  $page .= '<br/>';

  $page .= '<form action="'.url(OOSTPOORT_MEMBER_AGE).'" method="POST">';
  $page .= 'Leeftijd is <input id="text" name="age" size="3" maxlength="3" type="text" value="'.$age.'"> jaar ';
  $page .= '<input type="submit" value="'.t('Zoeken').'" />';
  $page .= '</form>';

  // Query Database
  $query   = 'select mid, voornamen, voegsel, achternaam, straatnaam, huisnummer, postcode, woonplaats, geboorte_datum, kerklidnr  from {oostpoort_members} where leeftijd='.$age.' ';
  $query  .= 'order by achternaam';
  $queryResult = db_query($query);

  $page .= '<table width=100% border=1 cellpadding=3 cellspacing=3>';

  // Show Banner
  $page .= '<tr><td>';
  if (user_access('access administration pages')) 
  {
     $page .= '<b>'.t('Id').'</b>';
     $page .= '</td><td>';
  }
  $page .= '<b>'.t('Foto').'</b>';
  $page .= '</td><td>';
  $page .= '<b>'.t('Naam').'</b>';
  $page .= '</td><td>';
  $page .= '<b>'.t('Straat').'</b>';
  $page .= '</td><td>';
  $page .= '<b>'.t('Plaats').'</b>';
  $page .= '</td><td>';
  $page .= '<b>'.t('Postcode').'</b>';
  $page .= '</td><td>';
  $page .= '<b>'.t('Geboorte datum').'</b>';
  $page .= '</td><td>';
  $page .= '<b>'.t('Telefoon').'</b>';
  $page .= '</td><td>';
  $page .= '<b>'.t('Email').'</b>';
  $page .= '</td></tr>';

  // Show all found members
  $page_tmp='';
  while ($data = db_fetch_object($queryResult))
  {
     $page_tmp .= '<tr><td width=50>';
     if (user_access('access administration pages')) 
     {
        $page_tmp .= $data->kerklidnr;
        $page_tmp .= '</td><td>';
     }

    $email=oostpoort_get_item2($data->kerklidnr,1);
    $telephone=oostpoort_get_item2($data->kerklidnr,2);
    $image=oostpoort_get_item2($data->kerklidnr,8);

    if (strlen($image)>0)
    {
      $page_tmp .= '<a href="'.url(OOSTPOORT_MEMBER_VIEW.'/'.$data->mid).'">';
      $filename=url(OOSTPOORT_THUMBNAILS_DIR.'/'.$image);
      $filename=str_replace(array("?q="), "", $filename);
      $page_tmp .= '<img src="'.$filename.'" width=25 height=30/></a>';
    }
    $page_tmp .= '</td><td>';
    $page_tmp .= l($data->voornamen.' '.$data->voegsel.' '.$data->achternaam, OOSTPOORT_MEMBER_VIEW.'/'.$data->mid);
    $page_tmp .= '</td><td>';
    $page_tmp .= $data->straatnaam.' '.$data->huisnummer;
    $page_tmp .= '</td><td>';
    $page_tmp .= $data->woonplaats;
    $page_tmp .= '</td><td>';
    $page_tmp .= $data->postcode;
    $page_tmp .= '</td><td>';
    list($year, $month, $day) = split('[/.-]', $data->geboorte_datum);
    $page_tmp .= $day.'-'.$month.'-'.$year;
    $page_tmp .= '</td><td>';
    $page_tmp .= $telephone;
    $page_tmp .= '</td><td>';
    $page_tmp .= l($email,'mailto:'.$email);
    $page_tmp .= '</td></tr>';
  }

  if ( $page_tmp!='') 
  {
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
