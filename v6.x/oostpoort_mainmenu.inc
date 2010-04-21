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
// MODULE MAIN MENU
// ##################################################################################

function oostpoort_mainmenu( ) 
{
  global $user;

  // key to encrypt crc checksum
  //oostpoort_makeCksum();

  // Breadcrumb menu
  $page  = '<div class="breadcrumb">';
  $page .= '<a href="'.url("").'">Home</a> > ';
  $page .= 'Menu';
  $page .= '</div>';

  $page .= '<div class="oostpoort">';

  drupal_set_title(t('Oostpoort Menu'));

  $page .= '<br/>';

  $page .= '<div class="admin-panel">';
  $page .= '<H3>';
  $page .= 'Keuze menu';
  $page .= '</H3>';
  $page .= '<div class="body">';
  $page .= '<p class="description">';
  $page .= 'Alle getoonde informatie (die niet aangepast kan worden) is afkomstig van het kerkelijk bureau. Mocht er iets niet kloppen neem dan contact op met het Goudse Hervormde kerkelijke bureau. Adres: Willem de Zwijgersingel 1, 2805 BP Gouda Telefoon:0182-513360 Email:kerkelijkbureau@hervormdgouda.nl';
  $page .= '</p>';
  $page .= '<dl class="admin-list">';

  $page .= '<dt>';
  $page .= '<a href="'.url(OOSTPOORT_FAMILY_LIST).'/A">Bekijk leden gegevens</a>';
  $page .= '</dt>';
  $page .= '<dd>';
  $page .= 'Extra informatie is nu nog niet beschikbaar';
  $page .= '</dd>';

  $page .= '<dt>';
  $page .= '<a href="'.url(OOSTPOORT_VISIT).'">Huis bezoek plannen</a>';
  $page .= '</dt>';
  $page .= '<dd>';
  $page .= 'Extra informatie is nu nog niet beschikbaar';
  $page .= '</dd>';

  $page .= '<dt>';
  $page .= '<a href="'.url(OOSTPOORT_MEMBER_AGE).'">Bepaalde leeftijdscategorie tonen</a>';
  $page .= '</dt>';
  $page .= '<dd>';
  $page .= 'Extra informatie is nu nog niet beschikbaar';
  $page .= '</dd>';

  $page .= '<dt>';
  $page .= '<a href="'.url(OOSTPOORT_JUBILEA_BIRTHDAY).'">Verjaardagsjubilea van dit jaar</a>';
  $page .= '</dt>';
  $page .= '<dd>';
  $page .= 'Extra informatie is nu nog niet beschikbaar';
  $page .= '</dd>';

  $page .= '<dt>';
  $page .= '<a href="'.url(OOSTPOORT_JUBILEA_MARRIED).'">Huwelijksjubilea van dit jaar</a>';
  $page .= '</dt>';
  $page .= '<dd>';
  $page .= 'Extra informatie is nu nog niet beschikbaar';
  $page .= '</dd>';

  $page .= '<br/>';
  $page .= '<br/>';

  $page .= '<dt>';
  $page .= '<a href="'.url(OOSTPOORT_UPLOAD).'">Upload een nieuw ledenbestand</a>';
  $page .= '</dt>';
  $page .= '<dd>';
  $page .= 'Extra informatie is nu nog niet beschikbaar';
  $page .= '</dd>';

  $page .= '</dl>';
  $page .= '</div>';
  $page .= '</div>';

  print theme("page", $page);
}

// ##################################################################################
// THE END
// ##################################################################################