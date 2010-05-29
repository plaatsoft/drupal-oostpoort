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

// ********************************************
// Debug methods
// ********************************************

/**
 * debug_param
 * Print on screen POSTED parameters
 */
function oostpoort_debug_param($id, $param) {
	//echo '['.$id.'='.$param.']';
}

/**
 * debug_param
 * Print on screen SQL commands
 */
function oostpoort_debug_sql($sql) {
	//echo '[Query='.$sql.']';
}


function oostpoort_show_array($array) {
    foreach ($array as $value) {
        if (is_array($value)) {
            show_array($value);
        } else {
            echo $value . "<br>";
        }
    }
}

// ********************************************
// View methods
// ********************************************

/**
 * View email
 */
function oostpoort_view_email($email) {
	return '<a href="mailto:'.$email.'">'.$email.'</a>';
}
 
/**
 * View Manitory & character
 */
function oostpoort_view_manitory() {
	return '<span class="form-required" title="'.t('Dit veld is verplicht').'"> *</span>';
}

/**
 * View Sex 
 */
function oostpoort_view_geslacht($label) {
   if ( $label=='M') {
		return t('Man');
   } else {
      return t('Vrouw');
   }
}
/** 
 * View text input field.
 */
function oostpoort_view_text($id, $size, $maxSize, $value, $readonly) {
	
	if ($readonly) {
		return $value;
	} else {	
		return '<input id="text" name="'.$id.'" size="'.$size.'" maxlength="'.$maxSize.'" type="text" value="'.$value.'">';
	}
}

/** 
 * Oostpoort form item.
 */
function oostpoort_form_item($label,$value,$description, $readonly) {
	
	if ($value!="") {
		
		$page .= '<div class="form-item">';
		
		$page .= '<label>';
		$page .= $label;
		$page .= '</label>';
		
		$page .= $value;
		
		if (($description!="") && !$readonly) {
			$page .= '<div class="description">';
			$page .= $description;
			$page .= '</div>';
		}
		
		$page .= '</div>';
	}
	return $page;
}
	
/**
 * View Checkbox type
 */
function oostpoort_view_checkbox( $id, $value, $readonly ) {

	if ( ! $readonly ) {
		if (($value=="WAAR") or ($value=="on")) {
			$page = '<input type="checkbox" name="'.$id.'" checked="true" />';
		} else {
			$page = '<input type="checkbox" name="'.$id.'" />';
		}
	} else {
		if (($value=="WAAR") or ($value=="on")) {
			$page = '<input type="checkbox" checked="true" disabled="true" />';
		} else {
			$page = '<input type="checkbox" disabled="true" />';				
		}
	}
	return $page;
}

/**
 * View drupal owner dropdown
 */
function oostpoort_view_owner($uid, $readonly) {

   global $user;

   if ( ! $readonly ) {
		$query  = 'SELECT uid, name FROM {users} order by name';
		$queryResult = db_query($query);     
		$data = db_fetch_object($queryResult);

		$page.='<select name="Owner">';

		$page.='<option value="0"';
			if ($uid==0) $page.='selected="selected" ';
		$page.='>&nbsp;</option>';
					
		while ($data = db_fetch_object($queryResult)) {
		
			$page.='<option value="'.$data->uid.'" ';
			if ($data->uid==$uid) $page.='selected="selected" ';
			$page.='>'.$data->name.' ['.$data->uid.']</option>';
		}
		$page.='</select>';
	  
   } else {
	 
      $query  = 'SELECT uid, name FROM {users} WHERE uid='.$uid;
      $queryResult = db_query($query);
      $data = db_fetch_object($queryResult);
		 
      $page .= $data->name;
   }
   return $page;
}

/**
 * View Block dropdown
 */
function oostpoort_view_block($blockId, $readonly, $omschrijving) {

   if ( ! $readonly ) {
		$query  = 'SELECT Blk_ID, Blk_Omschrijving FROM {oostpoort_blok} order by Blk_ID';
		$queryResult = db_query($query);     
		
		$page.='<select name="Block">';
		while ($data = db_fetch_object($queryResult)) {
					
			$page.='<option value="'.$data->Blk_ID.'" ';
			if ($data->Blk_ID==$blockId) $page.='selected="selected" ';
			if ($omschrijving) {
				$page.='>'.$data->Blk_Omschrijving.' [Blok '.$data->Blk_ID.']</option>';
			} else {
				$page.='>Blok '.$data->Blk_Id.'</option>';
			}		
		}
		$page.='</select> ';
	  
   } else {
	 
      $query  = 'SELECT Blk_ID, Blk_Omschrijving FROM {oostpoort_blok} WHERE Blk_ID='.$blockId;
      $queryResult = db_query($query);
      $data = db_fetch_object($queryResult);
		 
      $page .= $data->Blk_Omschrijving.' [Blok '.$data->Blk_Id.']';
   }
   return $page;
}

/**
 * View Role dropmenu
 */
function oostpoort_view_role($role, $readonly) {

   if ( ! $readonly ) {
		$query  = 'SELECT Rol_ID, Rol_Omschrijving FROM {oostpoort_rol} order by Rol_Omschrijving';
		$queryResult = db_query($query);     
		
		$page.='<select name="Role">';
		while ($data = db_fetch_object($queryResult)) {
		
			$page.='<option value="'.$data->Rol_ID.'" ';
			if ($data->Rol_ID==$role) $page.='selected="selected" ';
			$page.='>'.$data->Rol_Omschrijving.'</option>';
		}
		$page.='</select>';
	  
   } else {
	 
      $query  = 'SELECT Rol_ID, Rol_Omschrijving FROM {oostpoort_rol} WHERE Rol_ID='.$role;
      $queryResult = db_query($query);
      $data = db_fetch_object($queryResult);
		 
      $page .= $data->Rol_Omschrijving;
   }
   return $page;
}

/**
 * View Visitors dropdown
 */
function oostpoort_view_visiters($id, $value) {

	$query  = 'select ';
	$query .= '  concat(IF(b.Prs_Roepnaam="",b.Prs_Voornamen, b.Prs_Roepnaam)," ", b.Prs_Tussenvoegsels, " ", b.Prs_Achternaam) as Naam, '; 
	$query .= '  b.Prs_id as PrsId, ';
	$query .= '  a.Mdw_ActiefJN as Actief ';
	$query .= 'from ';
	$query .= '  oostpoort_medewerker a, oostpoort_persoon b ';
	$query .= 'where ';
	$query .= ' a.Mdw_Prs_id=b.Prs_id ';
	$query .= 'order by ';
	$query .= ' b.Prs_Achternaam';
  
	$queryResult = db_query($query); 
  
	$page .= '<select name="'.$id.'" onChange="submit();">';   
	while ($data = db_fetch_object($queryResult))
	{
		$page .= '<option value="'.$data->PrsId.'" ';
		if ($data->PrsId==$value) $page.='selected="selected" ';
		$page .= '>';
		if ($data->Actief=='ONWAAR') {
			$page .=$data->Naam.' ('.t('Niet Actief').')';
		} else {
			$page .='<b>'.$data->Naam.'</b>';
		}
		$page .= '</option>';
	}
	$page .= '</select>';
	return $page;
}

/**
 * View Date type
 */
function oostpoort_view_date($id, $value, $readonly) {
	
 	if ( ! $readonly)
 	{
 		return '<input type="text" name="'.$id.'" id="datepicker" size="11" maxlength="10" value="'.$value.'" >';	
 	} else {
 		return $value;
 	}
}
/**
 * View Note type
 */
function oostpoort_view_note($id, $value, $readonly) {

   	if ( $readonly ) {
   		if ($value!="") {
			return '<textarea class="form-textarea resizable" cols="100" rows="4" DISABLED>'.$value.'</textarea>';
   		}
	} else {
   		return '<textarea class="form-textarea resizable" name="'.$id.'" cols="100" rows="4" maxlength="1000">'.$value.'</textarea>';
	}
}

/**
 * View Visit Type dropdown 
 */
function oostpoort_view_type($id, $value, $readonly) {

   if ( ! $readonly ) {
		$query  = 'select Bzt_ID, Bzt_Omschrijving FROM {oostpoort_bezoektype} order by Bzt_ID';
		$queryResult = db_query($query);     
		
		$page.='<select name="VisitType">';
		while ($data = db_fetch_object($queryResult)) {
		
			$page.='<option value="'.$data->Bzt_ID.'" ';
			if ($data->Bzt_ID==$value) $page.='selected="selected" ';
			$page.='>'.$data->Bzt_Omschrijving.'</option>';
		}
		$page.='</select> ';
	  
   } else {
	 
      $query  = 'SELECT Bzt_Omschrijving FROM {oostpoort_bezoektype} WHERE Bzt_ID='.$value;
      $queryResult = db_query($query);
      $data = db_fetch_object($queryResult);
		 
      $page .= $data->Bzt_Omschrijving;
   }
   return $page;
}

function oostpoort_input_picture($readonly) {
	if (!$readonly) {
		$page = '<input name="uploadedfile" size="100" type="file" />'; 
	}
	return $page;
}

/**
 * Create hidden link 
 */
function oostpoort_hiddenlink($form,$key,$value,$action,$label) {
   return '<a href="#" onClick="hiddenInput(\''.$form.'\', \''.$key.'\', \''.$value.'\',\''.$action.'\');">'.$label.'</a>';
}

function oostpoort_breadcrumb($node) {
		
	$data = array();
	
	$tmp[] = l(t('Start'), "");
	$tmp[] = l(t('Leden administratie'),OOSTPOORT_MENU);
	
	if ($node["child1"]!="") {
		$tmp[] = $node["child1"];		
	}
	if ($node["child2"]!="") {
		$tmp[] = $node["child2"];		
	}
	if ($node["child3"]!="") {
		$tmp[] = $node["child3"];		
	}
	drupal_set_breadcrumb( $tmp );
}


function oostpoort_sortbar($Sort,$Action) {

	$sort_bar=array("A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P", "Q","R","S","T","U","V","W","X","Y","Z");
	for ($i=0; $i<26; $i++)
	{
		if ($i==0) $page .= '|';
		if ($Sort==$sort_bar[$i]) { 
			$page .= '<B>'.$Sort.'</B>|';
		} else {		
			$page .= oostpoort_hiddenlink('OostpoortForm', 'Sort', $sort_bar[$i], $Action, $sort_bar[$i]).'|';
		}
	}
	return $page;
}


// ********************************************
// Convert methods
// ********************************************

function oostpoort_offset_date($day) {

	list($year1, $month1, $day1) = split('-', date("Y-m-d"));
	$date1 = mktime(0,0,0,$month1,$day1,$year1);
	$date1 += (60*60*24*$day);
	$tmp= getdate($date1);
	return $tmp["year"].'-'.$tmp["mon"]."-".$tmp["mday"];
}
	
/**
 * View Date type
 */
function oostpoort_convert_date($date) { 

  $page = "";
  
  if ($date!=0) {
		list($year, $month, $day) = split('[/.-]', $date);
		$page = $day.'-'.$month.'-'.$year;
  }
  return $page;
}

/**
 * View Date Reverse type
 */
function oostpoort_convert_date_reverse($date) { 

  $page = "";
  
  if ($date!="") {
		list($day, $month, $year) = split('[/.-]', $date);
		$page = $year.'-'.$month.'-'.$day;
  }
  return $page;
}

// ********************************************
// Picture methods
// ********************************************

function oostpoort_check_picture($Group,$Id) {
	
	// Construct a valid drupal path for the named file
   $file=OOSTPOORT_THUMBNAILS_DIR.'/'.$Group.$Id.'.jpg';
   $target = file_create_path($file);

   // Check to see if the file exists
   if (file_exists($target)) {
   	  return true;
   } else {
      return false;
   }
}

/**
 * Load picture
 */
function oostpoort_load_picture($Group,$AdrId,$height,$width, $align, $lightbox) {
	
   // Construct a valid drupal path for the named file
   $file=OOSTPOORT_THUMBNAILS_DIR.'/'.$Group.$AdrId.'.jpg';
   $file_normal=OOSTPOORT_IMAGE_DIR.'/'.$Group.$AdrId.'.jpg';
   $target = file_create_path($file);
   $target_normal = file_create_path($file_normal);

   // Check to see if the file exists
   if (file_exists($target)) {	
		$filename=str_replace(array("?q="), "", url($file));
		$filename_normal=str_replace(array("?q="), "", url($file_normal));
		if ($lightbox) {
			$page.='<a href="'.$filename_normal.'" rel="lightbox" >';
		}
		$page.='<img align="'.$align.'" ';
		if ($width!=0) {
			$page.='width="'.$width.'" ';
		}
		if ($height!=0) {
			$page.='height="'.$height.'" ';
		}
		$page.='src="'.$filename.'" />';
		if ($lightbox) {
			$page.='</a>';
		}
	} 
	return $page;
}

/**
 * Save picture
 */
function oostpoort_save_picture($Group,$AdrId)
{
	// Fetch Image upload in a HTTP request
	if ($_FILES['uploadedfile']['name']!='') 
	{
		if ( (strstr($_FILES['uploadedfile']['name'],'.jpg')) || (strstr( $_FILES['uploadedfile']['name'],'.JPG')) )
		{
			$error='0';
			if(!is_dir(OOSTPOORT_IMAGE_DIR)) {
				if( !mkdir(OOSTPOORT_IMAGE_DIR) ) {
				
					$msg .= t('Creating image directory failed!');
					drupal_set_message($msg, 'error');
					
					$error='1';
				}
			}
			$filename= $Group.$AdrId.'.jpg';

			// Check dimensions of image
			$dimensions = getimagesize($_FILES['uploadedfile']['tmp_name']);
			$width  = $dimensions[0];
			$height = $dimensions[1];
			$resize = 0;

			// Limit picture 
			if ($width>$height) {
			
				if ($width>0) {
					$ratio = 800 / $width;
				}
				
				if ($ratio < 1) {
					$width=800;
					$height=$height*$ratio;
					$resize = 1;
				}
			} else  {
				if ($width>0) {
					$ratio = 600 / $width;
				}
				if ($ratio < 1) {
					$width=600;
					$height=$height*$ratio;
					$resize = 1;
				}
			}

			// Resize uploaded picture
			if ( $resize == 1 ) {
				if (!image_scale($_FILES['uploadedfile']['tmp_name'], file_create_path(OOSTPOORT_IMAGE_DIR.'/'.$filename), $width, $height)) {
				
					$msg .= t('Foto bestand te groot!');
					drupal_set_message($msg, 'error');
					
					$error='1';
				}
			} else {
				// Not resize needed, move file to storage place
				copy($_FILES['uploadedfile']['tmp_name'],OOSTPOORT_IMAGE_DIR.'/'.$filename);
			}

			if ($error==0)	{
				// Create thumbnails directory store
				if(!is_dir(OOSTPOORT_THUMBNAILS_DIR)) {
					if( !mkdir(OOSTPOORT_THUMBNAILS_DIR) )	{
					
						$msg .= t('Creating image directory failed!');
						drupal_set_message($msg, 'error');
					
						$error='1';
					}
				}

				$resize = 0;

				// Limit picture size on screen
				if ($width>$height) {
					if ($width>0) $ratio  = 160 / $width;
					if ($ratio < 1) {
						$width=160;
						$height=$height*$ratio;
						$resize = 1;
					}
				} else {
					if ($width>0) $ratio  = 120 / $width;
					if ($ratio < 1) {
						$width=120;
						$height=$height*$ratio;
						$resize = 1;
					}
				}

				// Create thumbnail picture
				// Resize uploaded picture
				if ( $resize == 1 ) {
					if (!image_scale(OOSTPOORT_IMAGE_DIR.'/'.$filename, file_create_path(OOSTPOORT_THUMBNAILS_DIR.'/'.$filename), $width, $height)) {
					
						$msg .= t('Foto bestand is te groot ');
						drupal_set_message($msg, 'error');
			
						$error='1';
					}
				} else {
					// Not resize needed, move file to storage place
					copy(OOSTPOORT_IMAGE_DIR.'/'.$filename, OOSTPOORT_THUMBNAILS_DIR.'/'.$filename);
				}

				if ($error=='0') {
				
					$msg .= t('Foto is opgeslagen.');
					drupal_set_message($msg, 'status');
				}
			}
		} else {
		
			$msg .= t('Alleen JPG foto formaat wordt ondersteund.');
			drupal_set_message($msg, 'status');
		}
	}
	
	// Clear uploaded file
	$_FILES['uploadedfile']['name']='';
}

// ********************************************
// Other methods
// ********************************************

/**
 * Call makeCksum once upon landing on the homepage
 */
function oostpoort_makeCksum() {
   $str = "";
   for ($i=0;$i<32;++$i) {
      $str .= chr(rand(32,126));
	}
   $_SESSION['Cksum'] = $str;
}

/**
 * Encode string
 * @return String
 */
function oostpoort_encode($x) {
    return base64_encode(substr($_SESSION['Cksum'],rand(0,28),4) . $x);
}

/**
 * Decode string
 * @return String
 */
function oostpoort_decode($x) {
    $y = base64_decode($x);
    if (strpos($_SESSION['Cksum'],substr($y,0,4)) === false) return false;
    return substr($y,4-strlen($y));
} 

/**
 * Function valid email address
 * @return true or false
 */
function oostpoort_validate_email($address) {

   return ! ereg("[A-Za-z0-9_-]+([\.]{1}[A-Za-z0-9_-]+)*@[A-Za-z0-9-]+([\.]{1}[A-Za-z0-9-]+)+",$address);
}

/*
 * Function valid number imput
 * @returns true if valid number (only numeric string), false if not
 */
function oostpoort_validate_number($str) {

  if (ereg('^[[:digit:]]+$', $str))
    return true;
  else
    return false;
}

function oostpoort_keep_selection() {
		
	foreach( $_POST as $var => $value) {
			
		list($active, $PrsId)=split("-",$var);
		if (($active=="Active") && ($value=="on")) {	
			$page .= '<input type="hidden" value="'.$value.'" name="'.$var.'" />';
		}
	}
	return $page;
}

function oostpoort_get_block($MdwId) {
	
	// Query Database
	$query  = '
		select 
			a.Blk_Id as BlkId 
		from 
			oostpoort_blok a, oostpoort_medewerker b
		where
			b.Mdw_Blk_ID=a.Blk_ID and
			b.Mdw_Id='.$MdwId;
	
	oostpoort_debug_sql($query);
	$queryResult = db_query($query);
  	$result = db_fetch_object($queryResult);
  	
  	$BlkId=$result->BlkId;	
  	return $BlkId;
}

// ********************************************
// Autorisation
// ********************************************

/**
 * Function check user access
 * @returns true if user is a active visit member else false 
 */
function oostpoort_visit_member_access() {
	
	global $user;
	 
	if ( !isset($_SESSION['MdwId']) )  { 
  
		$sql .= '
			select 
				Mdw_ID 
			from 
				oostpoort_medewerker 
			where 
				Mdw_ActiefJN="WAAR" and 
				Mdw_UID='.$user->uid;  
		oostpoort_debug_sql($sql);
		$queryResult = db_query($sql);
		$data = db_fetch_object($queryResult);
	
		$_SESSION['MdwId']=$data->Mdw_ID;
	}
	
	// Drupal admin has full access.
	if (user_access('access administration pages')) {
		return true;
	}
	
	if ($_SESSION['MdwId']!="") {	
		return true;
	} else {
		return false;
	}
}

/**
 * Function check user access
 * @returns true if user is active visit member admin
 */
function oostpoort_visit_member_admin_access() {
	
	global $user;
    	
	// Check medewerker admin access.
	if ( !isset($_SESSION['MdwAdmin']) )  { 
  
		$sql .= '
			select 
				Mdw_Admin as MdwAdmin
			from 
				oostpoort_medewerker 
			where 
				Mdw_ActiefJN="WAAR" and 
				Mdw_UID='.$user->uid;
		  
		oostpoort_debug_sql($sql);
		$queryResult = db_query($sql);
		$data = db_fetch_object($queryResult);
	
		$_SESSION['MdwAdmin']=$data->MdwAdmin;
	}
	
	// Drupal admin has full access.
	if (user_access('access administration pages')) {
		return true;
	}
	
	if ($_SESSION['MdwAdmin']=='WAAR') {		
		return true;
	} else {
		return false;
	}
}

// ##################################################################################
// THE END
// ##################################################################################
