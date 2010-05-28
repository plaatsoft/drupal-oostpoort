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
// Views
// ##################################################################################

function oostpoort_mainmenu( ) {

   //global user;
	
	// Clear session scope
	$_SESSION['Action']="";
	$_SESSION['Sort']="";
	$_SESSION['Search']="";
	$_SESSION['PrsId']="";
	$_SESSION['AdrId']="";
	
	// Breadcrumb menu
	oostpoort_breadcrumb('');

	$page .= '<div class="oostpoort">';

	drupal_set_title(t('Leden Administratie').' '.OOSTPOORT_VERSION);

	$page .= '<br/>';
	
	$page .= '<div class="admin-panel">';
	$page .= '<H3>';
	$page .= t('Keuze menu');
	$page .= '</H3>';
	$page .= '<br/>';
	
	$page .= '<div class="body">';
	
	$page .= '<dl class="admin-list">';
    
	$page .= '<dt>';
	$page .= '<a href="'.url(OOSTPOORT_FAMILY).'">Bekijk leden gegevens</a>';
	$page .= '</dt>';
	$page .= '<dd>';
	$page .= t('Bekijk adres gegevens en andere relevante informatie over leden. *');
	$page .= '</dd>';
  
   // Only active visit members can access this page.
	if (oostpoort_visit_member_access()) {
	
		$page .= '<dt>';
		$page .= '<a href="'.url(OOSTPOORT_MEMBER_AGE).'">Leeftijdscategorie rapport</a>';
		$page .= '</dt>';
		$page .= '<dd>';
		$page .= t('Dit rapport geeft een ledenlijst weer gebaseerd op leeftijdsgrenzen. *');
		$page .= '</dd>';

		$page .= '<dt>';
		$page .= '<a href="'.url(OOSTPOORT_JUBILEA_BIRTHDAY).'">Verjaardagsjubilaris rapport</a>';
		$page .= '</dt>';
		$page .= '<dd>';
		$page .= t('Dit rapport geeft alle verjaarsjubilea weer. *');
		$page .= '</dd>';

		$page .= '<dt>';
		$page .= '<a href="'.url(OOSTPOORT_JUBILEA_MARRIED).'">Huwelijksjubilaris rapport</a>';
		$page .= '</dt>';
		$page .= '<dd>';
		$page .= t('Dit rapport geeft alle huwelijksjubilea weer. *');
		$page .= '</dd>';
  
		$page .= '<dt>';
		$page .= '&nbsp;';
		$page .= '</dt>';

		$page .= '<dt>';
		$page .= '<a href="'.url(OOSTPOORT_VISIT).'">Bezoekwerk</a>';
		$page .= '</dt>';
		$page .= '<dd>';
		$page .= t('Bezoekmedewerkers kunnen hier bezoeken plannen en verwerken.');
		$page .= '</dd>';
	}
	
	// only active visit member admins can access these pages.
    if (oostpoort_visit_member_admin_access()) {
   
		$page .= '<dt>';
		$page .= '<a href="'.url(OOSTPOORT_VISIT_ADMIN).'">Bezoekwerk administratie</a>';
		$page .= '</dt>';
		$page .= '<dd>';
		$page .= t('Op deze pagina kunnen bezoekmedewerkers worden beheerder.');
		$page .= '</dd>';
  
		$page .= '<dt>';
		$page .= '<a href="'.url(OOSTPOORT_UPLOAD_CSV).'">Nieuw ledenbestand laden</a>';
		$page .= '</dt>';
		$page .= '<dd>';
		$page .= t('Op deze pagina kunnen nieuwe leden bestanden worden ingeladen.');
		$page .= '</dd>';
	}

	$page .= '<dt>';
	$page .= '&nbsp;';
	$page .= '</dt>';
		
	$page .= '<fieldset>' ;	
	$page .= '<legend>'.t('Opmerkingen *').'</legend>';
	$page .= '<div class="description">';
	$page .= t('Gegevens zijn bij gewerkte tot en met <b>'.variable_get('oostpoort_last_upload_date', '').'</b>');
	$page .= '<br/>';
	$page .= '<br/>';
	$page .= t('Alle getoonde informatie die niet aangepast kan worden is afkomstig van het ');
	$page .= t('hervormd kerkelijk bureau.<br/>Mocht er iets niet kloppen neem dan contact op met: ');
	$page .= '<br/>';
	$page .= '<br/>';
	$page .= '<b>'.t('Hervormd Kerkelijkbureau Gouda').'</b>';
	$page .= '<br/>';
	$page .= t('Willem de Zwijgersingel 1');
	$page .= '<br/>';
	$page .= t('2805 BP').'&nbsp;&nbsp;'.t('GOUDA'); 
	$page .= '<br/>';
	$page .= t('tel: 0182-513360');
	$page .= '<br/>';
	$page .= 'email : '.oostpoort_view_email( t('kerkelijkbureau@hervormdgouda.nl') );
	$page .= '</div>';
	$page .= '</fieldset>' ;
	
	$page .= '</dl>';
  
	$page .= '</div>';  
	$page .= '</div>';
	$page .= '</div>';

	print theme("page", $page);
}

// ##################################################################################
// THE END
// ##################################################################################