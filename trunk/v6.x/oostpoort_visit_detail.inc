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
// VISIT DETAIL 
// ##################################################################################

/**
 * Render a page showing visit detail page
 * @return HTML
 */
function oostpoort_visit_detail( $addressId=0 )  
{
  global $user;
  
  // Breadcrumb menu
  $page  = '<div class="breadcrumb">';
  $page .= '<a href="'.url("").'">Home</a> > ';
  $page .= '<a href="'.url(OOSTPOORT_MENU).'">Menu</a> > ';
  $page .= '<a href="'.url(OOSTPOORT_VISIT).'">Visit</a> > ';
  $page .= 'View';
  $page .= '</div>';

  $page .= '<div class="oostpoort">';

  drupal_set_title(t('Oostpoort bezoekwerk detail') );

  // Debugging value
  $border=0;

  $query  = 'SELECT ';
  $query .= 'concat (d.Adr_Straat," ", d.Adr_Huisnr) as straatnaam, ';
  $query .= 'd.Adr_Plaats as plaats, ';
  $query .= 'd.Adr_postcode as postcode, ';
  $query .= 'd.Adr_Telefoon as telefoon ';
  $query .= 'from oostpoort_adres d '; 
  $query .= 'where d.Adr_id='.$addressId;
  $queryResult = db_query($query);
  $data = db_fetch_object($queryResult);

  // Show address information in readonly mode
  $page .= '<br/>';

  $page .= '<table width="100%" cellpadding="1" >';
  $page .= '<tr>'; 
  $page .= '<td valign="top">';
  $page .= $data->straatnaam.'<br/>';
  $page .= $data->postcode.'&nbsp;&nbsp;'.$data->plaats.'<br/>';
  $page .= 'Nederland<br/>';
  $page .= $data->telefoon.'<br/>';
  $page .= '</td>';
  $page .= '</tr>';
  $page .= '</table>';

  $query  = 'SELECT ';
  $query .= 'a.Prs_GeslachtMV';
  $query .= 'concat (a.Prs_Voorletters," ", a.Prs_Tussenvoegsels, " ", a.Prs_Achternaam) as naam, ';
  $query .= 'from oostpoort_persoon a '; 
  $query .= 'where a.Prs_Huidig_Adr_ID='.$addressId;
  $queryResult = db_query($query);
  $data = db_fetch_object($queryResult);
  
  $page .= '<table width=100% cellpadding=1>';

  // Show Banner
  $page .= '<tr>';
  $page .= '<td><b>'.t('Id').'</b></td>';
  $page .= '<td><b>'.t('Foto').'</b></td>';
  $page .= '<td><b>'.t('Geslacht').'</b></td>';
  $page .= '<td><b>'.t('Naam').'</b></td>';
  $page .= '<td><b>'.t('Geboren').'</b></td>';
  $page .= '<td><b>'.t('Mobiel').'</b></td>';
  $page .= '<td><b>'.t('Email').'</b></td>';
  $page .= '</tr>';

  // Show all found members
  // Show all found address
  $page_tmp='';
  while ($data = db_fetch_object($queryResult))
  {
     //$email=oostpoort_get_item2($data->kerklidnr,1);
     //$mobile=oostpoort_get_item2($data->kerklidnr,3);
     //$image=oostpoort_get_item2($data->kerklidnr,8);

    $page .= $data->kerklidnr;
    $page .= '</td><td>';
    
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
  //while ($data = db_fetch_object($queryResult));



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