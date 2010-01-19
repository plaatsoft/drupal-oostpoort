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
// FAMILY MAP VIEW
// ##################################################################################

function oostpoort_family_map() 
{
  $parameters=$_SESSION["parameters"];

  // Breadcrumb menu
  $page  = '<div class="breadcrumb">';
  $page .= '<a href="'.url("").'">Home</a> > ';
  $page .= '<a href="'.url(OOSTPOORT_MENU).'">Menu</a> > ';
  $page .= '<a href="'.url(OOSTPOORT_FAMILY_LIST).'/'.$_SESSION["sort"].'">Family</a> > ';
  $page .= '<a href="'.url(OOSTPOORT_FAMILY_VIEW.'/'.$parameters).'">View</a> > ';
  $page .= 'Map';
  $page .= '</div>';

  $page .= '<div class="oostpoort">';

  drupal_set_title(t('Familie kaart'));

  // Fetch search parameter out HTTP request
  $street=$_POST['street'];
  $city=$_POST['city'];
  $country=$_POST['country'];

  drupal_set_html_head('<meta http-equiv="content-type" content="text/html; charset=UTF-8" />');
  drupal_set_html_head('<script type="text/javascript" language="javascript" src="http://api.maptp.map24.com/ajax?appkey='.variable_get('addressbook_map_key','').'"></script>');
  drupal_set_html_head('<script type="text/javascript">');

  drupal_set_html_head('function goMap24()');
  drupal_set_html_head('{');
  drupal_set_html_head('  Map24.loadApi( ["core_api", "wrapper_api"] , map24ApiLoaded );;');
  drupal_set_html_head('}');

  drupal_set_html_head('function map24ApiLoaded()');
  drupal_set_html_head('{');
  drupal_set_html_head('  Map24.MapApplication.init( { NodeName: "maparea" } );');
  drupal_set_html_head('}');

  drupal_set_html_head('function geocode( searchText )');
  drupal_set_html_head('{');
  drupal_set_html_head('  var geocoder = new Map24.GeocoderServiceStub();');
  drupal_set_html_head('  geocoder.geocode( searchText );');
  drupal_set_html_head('}');

  drupal_set_html_head('</script>');

  $page.='<body onload="goMap24()">';

   // Show menu Bar
  $page .= '<br/>';
  $page .= '<table width="100%" border=0>';
  $page .= '<tr>';

  $page .= '<td valign="left">';
  $page .= '<b>'.t('Adres: ').$street.', '.$city.', '.$country.'</b>';
  $page .= '</td>';

  $page .= '<td  valign="right">';
  $page.=' <input type="submit" value="'.t('Zoeken').'" onclick="return geocode(\''.$street.','.$city.','.$country.'\')">';
  $page .= '</td>';

  $page .= '<td valign="right">';
  $page .= '<form action="'.url(OOSTPOORT_FAMILY_VIEW.'/'.$parameters).'" method="POST">';
  $page .= '<input type="submit" name="commit" value="'.t('Return').'" />';
  $page .= '</form>';
  $page .= '</td>';

  $page .= '</tr>';
  $page .= '</table>';

  $page .= '<br/>';
  $page .= '<div id="maparea" style="width:700px;height:500px;background-color:#E0E0E0;"></div>';
  $page .= '</body>';

  $page .= '</div>';
  print theme("page", $page);
}

// ##################################################################################
// THE END
// ##################################################################################