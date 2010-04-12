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
// JUBILEA OVERVIEW
// ##################################################################################

function oostpoort_jubilea_married()  
{
  global $user;

  // Breadcrumb menu
  $page  = '<div class="breadcrumb">';
  $page .= '<a href="'.url("").'">Home</a> > ';
  $page .= '<a href="'.url(OOSTPOORT_MENU).'">Menu</a> > ';
  $page .= 'Huwelijk jubilea ';
  $page .= '</div>';

  $page .= '<div class="oostpoort">';

  drupal_set_title(t('Oostpoort huwelijk jubilea' ) );

  $page .= '<br/>';

  $x=array(12, 25, 40, 45, 50, 55, 60, 65, 70);
  foreach ($x as $value)
  {
    $page .= 'Leden die '.$value.' jaar getrouwd zijn<br/>';
    $year = date("Y")-$value;
    $query   = 'select mid, voornamen, voegsel, achternaam, straatnaam, huisnummer, postcode, woonplaats, geboorte_datum, huwelijk_datum, kerklidnr from {oostpoort_members} WHERE YEAR(huwelijk_datum)='.$year.' GROUP BY straatnaam, huisnummer, huwelijk_datum having ( COUNT(huwelijk_datum)=2) order by huwelijk_datum';

//select achternaam, straatnaam, huisnummer, huwelijk_datum from oostpoort_members as willem WHERE YEAR(huwelijk_datum)=1975
//GROUP BY straatnaam, huisnummer, huwelijk_datum HAVING ( COUNT(straatnaam) = 2 )

    $queryResult = db_query($query);
    $page .= show_data($queryResult,2);
  }

  $page .= '</div>';
  print theme("page", $page);
}

// ##################################################################################
// THE END
// ##################################################################################



