<?php
/**
 * Created by wplaat (Plaatsoft)
 *
 * This software is open source and may be copied, distributed or modified under the terms of the GNU General Public License (GPL) Version 2
 *
 * For more information visit the following website.
 * Website : http://www.plaatsoft.nl 
 *
 * Or send an email to the following address.
 * Email   : info@plaatsoft.nl
 */

// ##################################################################################
// SUPPORT FUNCTIONS
// ##################################################################################

// Call makeCksum once upon landing on the homepage
function oostpoort_makeCksum() {
       $str = "";
       for ($i=0;$i<32;++$i)
               $str .= chr(rand(32,126));
       $_SESSION['Cksum'] = $str;
}

function oostpoort_encode($x) {
    return base64_encode(substr($_SESSION['Cksum'],rand(0,28),4) . $x);
}

function oostpoort_decode($x) {
    $y = base64_decode($x);
    if (strpos($_SESSION['Cksum'],substr($y,0,4)) === false) return false;
    return substr($y,4-strlen($y));
} 

function oostpoort_view_picture($mid)
{
  $page='';
  if ($mid!=0)
  {
    // Query Database for Pictures
    $query  = 'SELECT iid, picture FROM {oostpoort_items} WHERE mid='.$mid;
    $queryResult = db_query($query);
    while ($data = db_fetch_object($queryResult))
    {
      // Show each picture with is found
      $page = '<a href="'.url(OOSTPOORT_PICTURE_VIEW.'/').$data->iid.'">';
      $filename=url(THUMBNAILS_DIR.'/'.$data->value);
      $filename=str_replace(array("?q="), "", $filename);
      $page.='<img align="right" src="'.$filename.'"/>';
      $page.='</a>';
    }
  }
  return $page;
}  

/**
 * Roles
 * This function will show all roles storing in the setting page.
 * @return HTML
 */
function oostpoort_view_roles($active_roles,$wanted_roles,$readonly) 
{
   global $user;
   $page='';

   $roles=split( ",", variable_get('oostpoort_roles',''));
   if ($roles[0]=='')
   {
      // No roles defined in setting page. return directly!
      return $page;
   }

   if (($readonly=='1') && ($active_roles=='') && ($wanted_roles=='')) {
      // Nothing to show
      return $page;
   }

   $page .= '<b>Gaven bank:</b><br/>';
   $page .= '<table border=0 width=100% cellpadding=1>';
   $page .='<tr>';
   $page .='<td valign="top" width=15%>';
   $page .=t('Actieve rollen').'&nbsp;';
   $page .='</td><td width=35%>';

   $first=1;
   $i=0;
   if ( $readonly=='0' ) {
     while ($roles[$i]!='') {

       if ( !strstr( $active_roles, $roles[$i] ) ) {
          $page.='<input type="checkbox" name="active_'.trim($roles[$i]).'">'.$roles[$i].'<br/>';
       }
       else {
         $page.='<input type="checkbox" name="active_'.trim($roles[$i]).'" CHECKED>'.$roles[$i].'<br/>';
       }
       $i++;
     }
   }
   else {
     while ($roles[$i]!='') {
       if ( strstr( $active_roles, $roles[$i] ) ) {
         $page.='<input type="checkbox" name="active_'.trim($roles[$i]).'" CHECKED DISABLED>'.$roles[$i];
         $page.='</input>';
         $page.='<br/>';
       }
       $i++;
     }
   }

   $page.='</td>';
   $page.='<td valign="top" width=15%>';
   $page.=t('Gewilde rollen').'&nbsp;';
   $page.='</td>';
   $page.='<td width=35%>';

   $first=1;
   $i=0;
   if ( $readonly=='0' ) {
     while ($roles[$i]!='') {

       if ( !strstr( $wanted_roles, $roles[$i] ) ) {
          $page.='<input type="checkbox" name="wanted_'.trim($roles[$i]).'">'.$roles[$i].'<br/>';
       }
       else {
         $page.='<input type="checkbox" name="wanted_'.trim($roles[$i]).'" CHECKED>'.$roles[$i].'<br/>';
       }
       $i++;
     }
   }
   else {
     while ($roles[$i]!='') {
       if ( strstr( $wanted_roles, $roles[$i] ) ) {
         $page.='<input type="checkbox" name="wanted_'.trim($roles[$i]).'" CHECKED DISABLED>'.$roles[$i];
         $page.='</input>';
         $page.='<br/>';
       }
       $i++;
     }
   }

   $page.='</td>';
   $page.='</tr>';
   $page.='</table>';
   return $page;
}

/*
 * View birthday field
 * @ birthday_day
 * @ birthday_month
 * @ birthday_year
 * @ return HTML
 */
function oostpoort_view_birthday($birthday_day,$birthday_month,$birthday_year,$readonly,$title) {

   global $user;

   $page.='<tr><td valign="top" width=15%>';
   $page.=t($title);
   $page.='</td><td>';

   if ( $readonly == '0' ) {
     $page.='<select name="birthday_day">';
     for ($i=1; $i<32; $i++)
     {
        $page.='<option value="'.$i.'" ';
        if ($birthday_day==$i) $page.='selected="selected" ';
        $page.='>'.$i.'</option>';
     }
     $page.='</select> ';

     $month=array("",t('January'),t('February'),t('March'),t('April'),t('May'),
       t('June'),t('July'),t('August'),t('September'),t('October'),t('November'),t('December'));

     $page.='<select name=".$title.">';
     for ($i=1; $i<13; $i++)
     {
        $page.='<option value="'.$i.'" ';
        if ($birthday_month==$i) $page.='selected="selected" ';
        $page.='>'.$month[$i].'</option>';
     }
     $page.='</select> ';

     if ( $readonly == '0' )
     {
       $current_year=date('Y');
       $page.='<select name="birthday_year">';
       for ($i=1900; $i<=$current_year; $i++) {
         $page.='<option value="'.$i.'" ';
         if ($birthday_year==$i) $page.='selected="selected" ';
         $page.='>'.$i.'</option>';
       }
       $page.='</select> ';
     }
   }
   else 
   {
     $birtday .= $birthday_day.'-'.$birthday_month.'-'.$birthday_year;
     $page .= '<input id="text" name="'.$title.'" size="11" type="text" value="'.$birtday.'" READONLY />';
   }

   $page.='</td></tr>';
   return $page;
}

/*
 * Function fill table line
 * @return HTML
 */
function oostpoort_view_line($first,$second) {

   $page .= "<tr><td valign='top' width=15%>".$first."</td><td>".$second."</td></tr>";
   return $page;
}

/*
 * Function valid email address
 * @return true or false
 */
function oostpoort_check_mail($adres) {

   return ! ereg("[A-Za-z0-9_-]+([\.]{1}[A-Za-z0-9_-]+)*@[A-Za-z0-9-]+([\.]{1}[A-Za-z0-9-]+)+",$adres);
}

/*
 * Function valid number imput
 * @returns true if valid number (only numeric string), false if not
 */
function oostpoort_check_number($str) {

  if (ereg('^[[:digit:]]+$', $str))
    return true;
  else
    return false;
}

/*
 * Function check user access
 * @returns true if access is allowed else false 
 */
function oostpoort_check_access($uid) {

  global $user;
  if ( ( (  ($user->uid==$uid) || 
            ($uid==0) ||  
            (variable_get('addressbook_wiki_edit_mode',0)==1)
         ) 
         && user_access('add address')
        ) 
        || user_access('access administration pages') 
     ) 
  {
    return true;
  }
  else
  {
    return false;
  }
}

function oostpoort_postcode($postcode)
{
  return substr($postcode,0,4).substr($postcode,5,2);
}

function oostpoort_achternaam($aanschrijfnaam)
{
  $start=strripos($aanschrijfnaam,".")+1;
  if ($start==1) $start=0;
  $end=strripos($aanschrijfnaam,"-")-1;
  if (($end==-1) || ($end<$start)) $end=strlen($aanschrijfnaam);
  $tmp=trim(substr($aanschrijfnaam,$start,($end-$start)));
  $start=strripos($tmp," ");
  $naam=trim(substr($tmp,$start));
  return $naam;
}

function oostpoort_date($data)
{
    list($day, $month, $year) = split('[/.-]', $data);
    return $year.'-'.$month.'-'.$day;
}

// ##################################################################################
// THE END
// ##################################################################################
