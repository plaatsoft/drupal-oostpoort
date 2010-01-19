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

function oostpoort_jubilea_birthday()  
{
  global $user;

  // Breadcrumb menu
  $page  = '<div class="breadcrumb">';
  $page .= '<a href="'.url("").'">Home</a> > ';
  $page .= '<a href="'.url(OOSTPOORT_MENU).'">Menu</a> > ';
  $page .= 'Verjaardag jubilea ';
  $page .= '</div>';

  $page .= '<div class="oostpoort">';

  drupal_set_title(t('Oostpoort verjaardag jubilea' ) );

  $page .= '<br/>';

  $x=array(50, 65, 70, 75, 80, 85, 90, 95);
  foreach ($x as $value)
  {
    $page .= 'Leden die '.$value.' jaar worden dit jaar<br/>';
    $year = date("Y")-$value;
    $query   = 'select mid, voornamen, voegsel, achternaam, straatnaam, huisnummer, postcode, woonplaats, geboorte_datum, kerklidnr 
      from {oostpoort_members} WHERE YEAR(geboorte_datum)='.$year.' order by geboorte_datum';
    $queryResult = db_query($query);
    $page .= show_data($queryResult,1);
  }

  // Query Database (100 jaar)
  $page .= 'Leden die 100 jaar en ouder worden dit jaar<br/>';
  $year = date("Y")-100;
  $query   = 'select mid, voornamen, voegsel, achternaam, straatnaam, huisnummer, postcode, woonplaats, geboorte_datum, kerklidnr 
from {oostpoort_members} WHERE (YEAR(geboorte_datum)<='.$year.' and YEAR(geboorte_datum)!=1900) order by geboorte_datum';
  $queryResult = db_query($query);
  $page .= show_data($queryResult,1);

  $page .= '</div>';
  print theme("page", $page);
}

// ##################################################################################
// THE END
// ##################################################################################