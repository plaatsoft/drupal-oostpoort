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

  // Breadcrumb menu
  $page  = '<div class="breadcrumb">';
  $page .= '<a href="'.url("").'">Start</a> > ';
  $page .= t('Oostpoort');
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
  $page .= '<a href="'.url(OOSTPOORT_FAMILY).'">Bekijk leden gegevens</a>';
  $page .= '</dt>';
  $page .= '<dd>';
  $page .= 'Dit rapport heeft informatie over de actuele Oostpoort leden.';
  $page .= '</dd>';
  
  $page .= '<dt>';
  $page .= '<a href="'.url(OOSTPOORT_MEMBER_AGE).'">Leeftijdscategorie rapport</a>';
  $page .= '</dt>';
  $page .= '<dd>';
  $page .= 'Dit rapport geeft de leden weer tussen twee geboortedatums.';
  $page .= '</dd>';

  $page .= '<dt>';
  $page .= '<a href="'.url(OOSTPOORT_JUBILEA_BIRTHDAY).'">Verjaardagsjubilea rapport</a>';
  $page .= '</dt>';
  $page .= '<dd>';
  $page .= 'Dit rapport geeft alle verjaarsjubilea weer van het lopende jaar.';
  $page .= '</dd>';

  $page .= '<dt>';
  $page .= '<a href="'.url(OOSTPOORT_JUBILEA_MARRIED).'">Huwelijksjubilea rapport</a>';
  $page .= '</dt>';
  $page .= '<dd>';
  $page .= 'Dit rapport geeft alle huwelijksjubilea weer van het lopende jaar.';
  $page .= '</dd>';
  
  $page .= '<br/>';
  $page .= '<br/>';

  $page .= '<dt>';
  $page .= '<a href="'.url(OOSTPOORT_VISIT).'">Bezoekwerk</a>';
  $page .= '</dt>';
  $page .= '<dd>';
  $page .= 'Bezoekmedewerkers kunnen hier bezoeken plannen en verwerken.';
  $page .= '</dd>';
  
  
  $page .= '<dt>';
  $page .= '<a href="'.url(OOSTPOORT_VISIT_ADMIN).'">Bezoekwerk Administratie</a>';
  $page .= '</dt>';
  $page .= '<dd>';
  $page .= 'Op deze pagina kunnen bezoekmedewerkers worden beheerder. (Admin rechten nodig)';
  $page .= '</dd>';
 
  $page .= '<br/>';
  $page .= '<br/>';  
  
  $page .= '<dt>';
  $page .= '<a href="'.url(OOSTPOORT_UPLOAD).'">Upload een nieuw ledenbestand</a>';
  $page .= '</dt>';
  $page .= '<dd>';
  $page .= 'Op deze pagina kunnen nieuwe leden bestanden worden ingeladen. (Admin rechten nodig)';
  $page .= '</dd>';

  $page .= '</dl>';
  $page .= '</div>';
  $page .= '</div>';

  print theme("page", $page);
}

// ##################################################################################
// THE END
// ##################################################################################