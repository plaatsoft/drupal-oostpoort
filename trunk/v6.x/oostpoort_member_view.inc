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
// MEMBER VIEW
// ##################################################################################

/**
 * Render a page showing the selected member in readonly mode
 * @return HTML
 */
function oostpoort_member_view( $mid=0 )  {

  global $user;

  $parameters=$_SESSION["parameters"];

  // Breadcrumb menu
  $page  = '<div class="breadcrumb">';
  $page .= '<a href="'.url("").'">Home</a> > ';
  $page .= '<a href="'.url(OOSTPOORT_MENU).'">Menu</a> > ';
  $page .= '<a href="'.url(OOSTPOORT_FAMILY_LIST).'/'.$_SESSION["sort"].'">Family</a> > ';
  $page .= '<a href="'.url(OOSTPOORT_FAMILY_VIEW.'/'.$parameters).'">View</a> > ';
  $page .= 'Member';
  $page .= '</div>';

  $page .= '<div class="oostpoort">';

  // Get search value out session scope
  $parameters=$_SESSION["parameters"];

  // Fetch data from HTTP request
  $commit = isset($_POST['commit']) ? '1' : '0';

  if ($commit=='1') 
  {
    // Insert extra member information in database
    $email=$_POST['email'];
    $mobile=$_POST['mobile'];
    $telephone=$_POST['telephone'];
    $work=$_POST['work'];
    $notes=$_POST['notes'];

    /* Fetch all active and wanted roles from the HTTP request */
    $i=0;
    $active_first=1;
    $tmp=split( ",", variable_get('oostpoort_roles',''));
    while ($tmp[$i]!='') 
    {
      if ($_POST["active_$tmp[$i]"]=='on')
      {
         if ($active_first=='1') 
         {
            $active_roles=$tmp[$i];
            $active_first=0;
         }
         else $active_roles.=';'.$tmp[$i];
      }
      $i++;
    }

    oostpoort_set_item($mid,1,$email);
    oostpoort_set_item($mid,2,$telephone);
    oostpoort_set_item($mid,3,$mobile);
    oostpoort_set_item($mid,4,$work);
    oostpoort_set_item($mid,5,$notes);
    oostpoort_set_item($mid,6,$active_roles);
  }
  else
  { 
    // Fetch extra member information out database
    $email        = oostpoort_get_item($mid,1);
    $telephone    = oostpoort_get_item($mid,2);
    $mobile       = oostpoort_get_item($mid,3);
    $work         = oostpoort_get_item($mid,4);
    $notes        = oostpoort_get_item($mid,5);
    $active_roles = oostpoort_get_item($mid,6);
    $wanted_roles = oostpoort_get_item($mid,7);
    $image        = oostpoort_get_item($mid,8);
  }

  // Show information on screen
  drupal_set_title(t('Oostpoort lid overzicht') );

  // Fetch member from database
  $query  = 'SELECT geslacht, aanschrijf_naam, voornamen, straatnaam, huisnummer, postcode, woonplaats, geboorte_datum,';
  $query .= 'geboorte_plaats, doop_datum, doop_plaats, burgelijkestaat, kerkelijkestaat, huwelijk_datum, huwelijk_plaats, ';
  $query .= 'belijdenis_datum, belijdenis_plaats, herkomst_datum, herkomst_plaats, herkomst_datum, herkomst_plaats, ';
  $query .= 'mutatie_datum, sectie, wijknaam, kerkelijk_relatie, voorkeur_wijk, kerkblad, kerklidnr, last_updated FROM {oostpoort_members} ';
  $query .= 'WHERE mid='.$mid;
  $queryResult = db_query($query);
  $data = db_fetch_object($queryResult);

  $page .= '<form action="'.url(OOSTPOORT_MEMBER_VIEW.'/').$mid.'" method="POST">';
  $page .= "<br/><b>Algemene gegevens:</b><br/>";
  $page .= '<table cellpadding=2 width=100% >';

  $page .= '<tr><td valign="top" width=15%>';
  $page .= t('Geslacht');
  $page .= '</td><td>';
  $page .= '<input id="text" name="doop_plaats" size="50" type="text" value="'.$data->geslacht.'" READONLY />';
  $page .= '</td>';

  if (strlen($image)>=0) 
  {
    $page .= '<td ROWSPAN=9 valign="top">';
    $page .= '<a href="'.url(OOSTPOORT_PICTURE_VIEW.'/').$mid.'">';
    $page.='<img align="right" src="'.url(OOSTPOORT_THUMBNAILS_DIR.'/'.$image).'"/>';
    $page .= '</a>';
    $page .= '</td>';
  }
  $page .= '</tr>';

  $page .= oostpoort_view_line(t('Aanschrijfnaam'),'<input id="text" name="aanschrijfnaam" size="50" type="text" value="'.$data->aanschrijf_naam.'" READONLY />');
  $page .= oostpoort_view_line(t('Voornamen'),'<input id="text" name="voornamen" size="50" type="text" value="'.$data->voornamen.'" READONLY />');
  $page .= oostpoort_view_line(t('Straat'),'<input id="text" name="straat" size="50" type="text" value="'.$data->straatnaam.' '.$data->huisnummer.'" READONLY />');

  $page .= oostpoort_view_line(t('postcode'),'<input id="text" name="postcode" size="8" type="text" value="'.$data->postcode.'" READONLY />');
  $page .= oostpoort_view_line(t('woonplaats'),'<input id="text" name="woonplaats" size="50" type="text" value="'.$data->woonplaats.'" READONLY />');
  $page .= oostpoort_view_line(t('land'),'<input id="text" name="land" size="50" type="text" value="Nederland" READONLY />');

  list($year, $month, $day) = split('[/.-]', $data->geboorte_datum);
  $page .= oostpoort_view_birthday($day,$month,$year,'1',"Geboorte datum");

  if ($data->kerklidnr!=0)
  { 
     $page .= oostpoort_view_line(t('Telefoon'),'<input id="text" name="telephone" size="16" type="text" value="'.$telephone.'" />');
     $page .= oostpoort_view_line(t('Mobiel'),'<input id="text" name="mobile" size="16" type="text" value="'.$mobile.'" />');
     $page .= oostpoort_view_line(t('Email'),'<input id="text" name="email" size="50" type="text" value="'.$email.'" />');
     $page .= oostpoort_view_line(t('Werk'),'<textarea id="work" cols=55 rows=4 name="work" >'.$work.'</textarea>');
     $page .= oostpoort_view_line(t('Overige'),'<textarea id="notes" cols=55 rows=4 name="notes" >'.$notes.'</textarea>');
     $page .= '</tr></table>';
     $page .= oostpoort_view_roles($active_roles,$wanted_roles, '0');
  }
  else
  {  
     $page .= oostpoort_view_line(t('Telefoon'),'<input id="text" name="telephone" size="16" type="text" value="'.$telephone.'" READONLY />');
     $page .= oostpoort_view_line(t('Mobiel'),'<input id="text" name="mobile" size="16" type="text" value="'.$mobile.'" READONLY />');
     $page .= oostpoort_view_line(t('Email'),'<input id="text" name="email" size="50" type="text" value="'.$email.'" READONLY />');
     $page .= oostpoort_view_line(t('Werk'),'<textarea id="work" cols=55 rows=4 name="work" READONLY >'.$work.'</textarea>');
     $page .= oostpoort_view_line(t('Overige'),'<textarea id="notes" cols=55 rows=4 name="notes" READONLY>'.$notes.'</textarea>');
     $page .= '</tr></table>';
     $page .= oostpoort_view_roles($active_roles,$wanted_roles, '1');
  }

  // Show this information only for user with "view all" rights.
  if ( user_access('view all') ) 
  {
   $page .= "<b>Kerkelijke administratie gegevens:</b><br/>";
   $page .= '<table cellpadding=2 width=50%>';

   $page .= oostpoort_view_line(t('Kerklidnr'),'<input id="text" name="mid" size="10" type="text" value="'.$data->kerklidnr.'" READONLY />');
   $page .= oostpoort_view_line(t('Mid'),'<input id="text" name="mid" size="10" type="text" value="'.$mid.'" READONLY />');
   $page .= oostpoort_view_line(t('Last updated'),'<input id="text" name="last_updated" size="22" type="text" value="'.$data->last_updated.'" READONLY />');

   $page .= oostpoort_view_line(t('Geboorte plaats'),'<input size="50" type="text" value="'.$data->geboorte_plaats.'" READONLY />');

   list($year, $month, $day) = split('[/.-]', $data->doop_datum);
   $page .= oostpoort_view_birthday($day,$month,$year,'1',"Doop datum");

   $page .= oostpoort_view_line(t('Doop plaats'),'<input name="doop_plaats" size="50" type="text" value="'.$data->doop_plaats.'" READONLY />');
   $page .= oostpoort_view_line(t('Burgelijke staat'),'<input name="burgelijkstaat" size="50" type="text" value="'.$data->burgelijkestaat.'" READONLY />');
   $page .= oostpoort_view_line(t('Kerkelijke staat'),'<input name="kerkelijkstaat" size="50" type="text" value="'.$data->kerkelijkestaat.'" READONLY />');

   list($year, $month, $day) = split('[/.-]', $data->huwelijk_datum);
   $page .= oostpoort_view_birthday($day,$month,$year,'1',"Huwelijk datum");

   $page .= '<tr><td valign="top" width=15%>';
   $page .= t('Kerkelijke gezindte');
   $page .= '</td><td>';
   $page .= '<input id="text" name="kerkelijke_gezindte" size="50" type="text" value="'.$data->huwelijk_plaats.'" READONLY />';
   $page .= '</td></tr>';

   $page .= '<tr><td valign="top" width=15%>';

   list($year, $month, $day) = split('[/.-]', $data->belijdenis_datum);
   $page .= oostpoort_view_birthday($day,$month,$year,'1',"Belijdenis datum");
 
   $page .= '<tr><td valign="top" width=15%>';
   $page .= t('Belijdenis plaats');
   $page .= '</td><td>';
   $page .= '<input id="text" name="belijdenis_plaats" size="50" type="text" value="'.$data->belijdenis_plaats.'" READONLY />';
   $page .= '</td></tr>';
 
   list($year, $month, $day) = split('[/.-]', $data->herkomst_datum);
   $page .= oostpoort_view_birthday($day,$month,$year,'1',"Herkomst datum");

   $page .= '<tr><td valign="top" width=15%>';
   $page .= t('Herkomst plaats');
   $page .= '</td><td>';
   $page .= '<input id="text" name="herkomst_plaats" size="50" type="text" value="'.$data->herkomst_plaats.'" READONLY />';
   $page .= '</td></tr>';

   $page .= '<tr><td valign="top" width=15%>';
   $page .= t('Wijknaam');
   $page .= '</td><td>';
   $page .= '<input id="text" name="wijknaam" size="50" type="text" value="'.$data->wijknaam.'" READONLY />';
   $page .= '</td></tr>';

   $page .= '<tr><td valign="top" width=15%>';
   $page .= t('Sectie');
   $page .= '</td><td>';
   $page .= '<input id="text" name="sectie" size="50" type="text" value="'.$data->sectie.'" READONLY />';
   $page .= '</td></tr>';

   list($year, $month, $day) = split('[/.-]', $data->mutatie_datum);
   $page .= oostpoort_view_birthday($day,$month,$year,'1',"Mutatie datum");

   $page .= '<tr><td valign="top" width=15%>';
   $page .= t('Kerkelijke relatie');
   $page .= '</td><td>';
   $page .= '<input id="text" name="kerkelijke_relatie" size="50" type="text" value="'.$data->kerkelijke_relatie.'" READONLY />';
   $page .= '</td></tr>';

   $page .= '<tr><td valign="top" width=15%>';
   $page .= t('Voorkeur Wijk');
   $page .= '</td><td>';
   $page .= '<input id="text" name="voorkeur_wijk" size="50" type="text" value="'.$data->voorkeur_wijk.'" READONLY />';
   $page .= '</td></tr>';

   $page .= '<tr><td valign="top" width=15%>';
   $page .= t('Kerkblad');
   $page .= '</td><td>';
   $page .= '<input id="text" name="kerkblad" size="50" type="text" value="'.$data->kerkblad.'" READONLY />';
   $page .= '</td></tr>';

   $page .= '</table>';
  }

  // Show menu Bar
  $page .= '<br/>';
  $page .= '<table align="left">';
  $page .= '<tr>';

  if ($data->kerklidnr!=0)
  { 
    $page .= '<td valign="top">';
    $page .= '<tr><td valign="top">';
    $page .= '<input type="hidden" name="mid" value="'.$mid.'" />';
    $page .= '<input type="submit" name="commit" value="'.t('Opslaan').'" />';
  }
  $page .= '</form>';
  $page .= '</td>';

  if (($data->kerklidnr!=0) && (strlen($image)==0))
  {
      $page .= '<td valign="top">';
      $page .= '<form action="'.url(OOSTPOORT_PICTURE_EDIT.'/').$mid.'" method="POST">';
      $page .= '<input type="hidden" name="fid" value="'.$fid.'" />';
      $page .= '<input type="submit" value="'.t('Pasfoto toevoegen').'" />';
      $page .= '</form>';
      $page .= '</td>';
  }

  $page .= '<td valign="top">';
  $page .= '<form action="'.url(OOSTPOORT_FAMILY_VIEW.'/').$parameters.'" method="POST">';
  $page .= '<input type="submit" name="commit" value="'.t('Terug').'" />';
  $page .= '</form>';
  $page .= '</td>';

  $page .= '</tr>';
  $page .= '</table>';

  $page .= '<br/>';
  $page .= '<br/>';

  $page .= '</div>';
  print theme("page", $page);
}

// ##################################################################################
// THE END
// ##################################################################################