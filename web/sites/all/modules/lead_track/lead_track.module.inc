<?php 


/**
 * Format string to the specific front short url format : /l/[slug]/[utm_content]
 *
 * @param string $slug
 *
 * @param string $utm_content
 *
 * @return string
 * 	The formated short url
 */
function _lead_track_format_url($slug, $utm_content){

	global $base_url;
	$url_format = variable_get(LEAD_TRACK_V_GLOBAL_URL_FORMAT, LEAD_TRACK_D_GLOBAL_URL_FORMAT);

	return $base_url . '/' . $url_format . '/' . $slug . '/' . $utm_content;
}

/**
 * Return the percentage
 *
 * @param int $total
 *
 * @param int $current
 *
 * @return mixed
 * 	% format if value are correct
 *  string 'n/a' if not
 */
function _lead_track_format_percentage($total, $current, $is_percent = true) {

	if (!$total){
		$percentage = 'n/a';
		$is_percent = false;
	}
	else if($total == $current) {
		// If $total doesn't evaluate as true or is equal to the current set, then
		// we're finished, and we can return "100".
		$percentage = '100';
	}
	else {
		// We add a new digit at 200, 2000, etc. (since, for example, 199/200
		// would round up to 100% if we didn't).
		$decimal_places = max(0, floor(log10($total / 2.0)) - 1);
		do {
			// Calculate the percentage to the specified number of decimal places.
			$percentage = sprintf('%01.' . $decimal_places . 'f', round($current / $total * 100, $decimal_places));
			// When $current is an integer, the above calculation will always be
			// correct. However, if $current is a floating point number (in the case
			// of a multi-step batch operation that is not yet complete), $percentage
			// may be erroneously rounded up to 100%. To prevent that, we add one
			// more decimal place and try again.
			$decimal_places++;
		} while ($percentage == '100');
	}
	return ($is_percent) ? $percentage.'%': $percentage;
}


function nicetime($date)
{
	if(empty($date)) {
		return "No date provided";
	}

	$periods         = array("second", "minute", "hour", "day", "week", "month", "year", "decade");
	$lengths         = array("60","60","24","7","4.35","12","10");

	$now             = time();
	$unix_date         = $date;

	// check validity of date
	if(empty($unix_date)) {
		return "Bad date";
	}

	// is it future date or past date
	if($now > $unix_date) {
		$difference     = $now - $unix_date;
		$tense         = "ago";

	} else {
		$difference     = $unix_date - $now;
		$tense         = "from now";
	}

	for($j = 0; $difference >= $lengths[$j] && $j < count($lengths)-1; $j++) {
		$difference /= $lengths[$j];
	}

	$difference = round($difference);

	if($difference != 1) {
		$periods[$j].= "s";
	}

	return "$difference $periods[$j] {$tense}";
}

// Handle the parsing of the _ga cookie
function parseCookie()
{
	list($version,$domainDepth, $cid1, $cid2) = split('[\.]', $_COOKIE["_ga"],4);
	return array('version' => $version, 'domainDepth' => $domainDepth, 'cid' => $cid1.'.'.$cid2);
}

// Get cid
function getCid()
{
	$contents = parseCookie();
	return $contents['cid'];
}

function formatSeconds($seconds){
	$SECONDS_IN_DAY = 24 * 3600;
	$SECONDS_IN_HOUR = 3600;
	$SECONDS_IN_MINUTE = 60;

	$result = "";

	// days
	if ($seconds >= $SECONDS_IN_DAY)
	{
		$days = floor($seconds/$SECONDS_IN_DAY);
		$result .= $days . (($days > 1)? ' days ' : 'day ');
		$seconds = $seconds % $SECONDS_IN_DAY;
	}

	// hours
	if ($seconds >= $SECONDS_IN_HOUR)
	{
		$hours = floor($seconds/$SECONDS_IN_HOUR);
		$result .= ($hours < 10)? '0'.$hours : $hours;
		$seconds = $seconds % $SECONDS_IN_HOUR;
	}else{
		$result = "00";
	}

	$result .= ":";
	// minutes
	if ($seconds >= $SECONDS_IN_MINUTE)
	{
		$minutes = floor($seconds/$SECONDS_IN_MINUTE);
		$result .= ($minutes < 10)? '0'.$minutes : $minutes;
		$seconds = $seconds % $SECONDS_IN_MINUTE;
	}else{
		$result .= "00";
	}

	$result .= ":";
	// seconds
	if ($seconds > 0){
		$result .= ($seconds < 10)? '0'.$seconds : $seconds;
	}else{
		$result .= "00";
	}

	return $result;
}

function orderCampaignRows($current, $next)
{
	$current_value = $current->c[1]->v;
	$next_value = $next->c[1]->v;

	if ($current_value == $next_value) {
		return 0;
	}
	return ($current_value > $next_value) ? -1 : 1;
}

function orderCampaignRowsByScore($current, $next)
{
	$current_value = $current['data'][8]['data'];
	$next_value = $next['data'][8]['data'];

	if ($current_value == $next_value) {
		return 0;
	}
	return ($current_value < $next_value) ? -1 : 1;
}

function orderCampaignRowsByRank($current, $next)
{
	$current_value = $current['data'][9]['data'];
	$next_value = $next['data'][9]['data'];

	if ($current_value == $next_value) {
		return 0;
	}
	return ($current_value < $next_value) ? -1 : 1;
}

function orderRowsByScore($current, $next)
{
	$current_value = $current['data'][11]['data'];
	$next_value = $next['data'][11]['data'];

	if ($current_value == $next_value) {
		return 0;
	}
	return ($current_value < $next_value) ? -1 : 1;
}

function orderRowsByRank($current, $next)
{
	$current_value = $current['data'][12]['data'];
	$next_value = $next['data'][12]['data'];

	if ($current_value == $next_value) {
		return 0;
	}
	return ($current_value < $next_value) ? -1 : 1;
}

function getUserIP()
{
	$client  = @$_SERVER['HTTP_CLIENT_IP'];
	$forward = @$_SERVER['HTTP_X_FORWARDED_FOR'];
	$remote  = $_SERVER['REMOTE_ADDR'];
	$ip = NULL;

	if(filter_var($client, FILTER_VALIDATE_IP))
	{
		$ip = $client;
	}
	elseif(filter_var($forward, FILTER_VALIDATE_IP))
	{
		$ip = $forward;
	}
	else
	{
		$ip = $remote;
	}

	return $ip;
}

function _lead_track_get_total_visits($start_date = NULL, $end_date = NULL){
	//Use Database API to retrieve current posts.
	$query = db_select(CONST_SQL_TABLE_VISIT, 'v');
	$query->addExpression('COUNT(v.vid)', 'total');

	if($start_date != NULL && $end_date != NULL){
		$query->condition(db_and()->condition('v.timestamp', $start_date,'>')->condition('v.timestamp', $end_date,'<'));
	}

	$return = $query->execute()->fetchAssoc();
	return $return['total'];
}

function _lead_track_get_total_visits_campaign($urls, $start_date = NULL, $end_date = NULL){
	//Use Database API to retrieve current posts.
	$query = db_select(CONST_SQL_TABLE_VISIT, 'v');
	$query->addExpression('COUNT(v.vid)', 'total');

	$u = array();

	foreach ($urls as $key => $url) {
		$u[] = $url->uid;
	}
	
	if($start_date != NULL && $end_date != NULL){
		$query->condition(db_and()->condition('v.url_id', $u, 'IN')->condition('v.timestamp', $start_date, '>')->condition('v.timestamp', $end_date, '<'));
	}else{
		$query->condition('v.url_id', $u, 'IN');
	}

	$return = $query->execute()->fetchAssoc();
	return $return['total'];
}

function _lead_track_get_total_visits_url($url, $start_date = NULL, $end_date = NULL){
	//Use Database API to retrieve current posts.
	$query = db_select(CONST_SQL_TABLE_VISIT, 'v');
	$query->addExpression('COUNT(v.vid)', 'total');

	if($start_date != NULL && $end_date != NULL){
		$query->condition(db_and()->condition('v.url_id', $url->uid)->condition('v.timestamp', $start_date, '>')->condition('v.timestamp', $end_date, '<'));
	}else{
		$query->condition('v.url_id', $url->uid);
	}

	$return = $query->execute()->fetchAssoc();
	return $return['total'];
}
function _lead_track_get_total_visits_user($user_id, $start_date = NULL, $end_date = NULL){
	//Use Database API to retrieve current posts.
	$query = db_select(CONST_SQL_TABLE_VISIT, 'v');
	$query->addExpression('COUNT(v.vid)', 'total');

	if($start_date != NULL && $end_date != NULL){
		$query->condition(db_and()->condition('v.uid', $user_id)->condition('v.timestamp', $start_date, '>')->condition('v.timestamp', $end_date, '<'));
	}else{
		$query->condition('v.uid', $user_id);
	}

	$return = $query->execute()->fetchAssoc();
	return $return['total'];
}

function _lead_track_get_total_links($start_date = NULL, $end_date = NULL){
	//Use Database API to retrieve current posts.
	$query = db_select(CONST_SQL_TABLE_URL, 'u')
	->fields('u', array('uid'));
	$query->addExpression('COUNT(u.uid)', 'total');

	if($start_date != NULL && $end_date != NULL){
		$query->condition(db_and()->condition('u.created', $start_date,'>')->condition('u.created', $end_date,'<'));
	}

	$return = $query->execute()->fetchAssoc();
	return $return['total'];
}

function _lead_track_get_total_links_campaign($urls, $start_date = NULL, $end_date = NULL){
	//Use Database API to retrieve current posts.
	$query = db_select(CONST_SQL_TABLE_URL, 'u')
	->fields('u', array('uid'));
	$query->addExpression('COUNT(u.uid)', 'total');

	$id_campaign = 0;

	foreach ($urls as $key => $url) {
		$id_campaign = $url->cid;
	}

	if($start_date != NULL && $end_date != NULL){
		$query->condition(db_and()->condition('u.cid', $id_campaign, '=')->condition('u.created', $start_date, '>')->condition('u.created', $end_date, '<'));
	}else{
		$query->condition('u.cid', $id_campaign, '=');
	}

	$return = $query->execute()->fetchAssoc();
	return $return['total'];
}

function _lead_track_get_total_campaigns($start_date = NULL, $end_date = NULL){
	//Use Database API to retrieve current posts.
	$query = db_select(CONST_SQL_TABLE_CAMPAIGN, 'c')
	->fields('c', array('cid'));
	$query->addExpression('COUNT(c.cid)', 'total');

	if($start_date != NULL && $end_date != NULL){
		$query->condition(db_and()->condition('c.created', $start_date,'>')->condition('c.created', $end_date,'<'));
	}

	$return = $query->execute()->fetchAssoc();
	return $return['total'];
}

function _lead_track_get_total_events($start_date = NULL, $end_date = NULL){
	//Use Database API to retrieve current posts.
	$query = db_select(CONST_SQL_TABLE_EVENT, 'e')
	->fields('e', array('eid'));
	$query->addExpression('COUNT(e.eid)', 'total');

	if($start_date != NULL && $end_date != NULL){
		$query->condition(db_and()->condition('e.created', $start_date,'>')->condition('e.created', $end_date,'<'));
	}

	$return = $query->execute()->fetchAssoc();
	return $return['total'];
}

function _lead_track_get_total_events_campaign($urls, $start_date = NULL, $end_date = NULL){
	//Use Database API to retrieve current posts.
	$query = db_select(CONST_SQL_TABLE_EVENT, 'e')
	->fields('e', array('eid'));
	$query->addExpression('COUNT(e.eid)', 'total');

	$slugs = array();

	foreach ($urls as $key => $url) {
		$slugs[] = $url->slug;
	}

	if($start_date != NULL && $end_date != NULL){
		$query->condition(db_and()->condition('e.slug', $slugs, 'IN')->condition('e.created', $start_date, '>')->condition('e.created', $end_date, '<'));
	}else{
		$query->condition('e.slug', $slugs, 'IN');
	}


	$return = $query->execute()->fetchAssoc();
	return $return['total'];
}

function _lead_track_get_total_events_url($url, $start_date = NULL, $end_date = NULL){
	//Use Database API to retrieve current posts.
	$query = db_select(CONST_SQL_TABLE_EVENT, 'e')
	->fields('e', array('eid'));
	$query->addExpression('COUNT(e.eid)', 'total');

	if($start_date != NULL && $end_date != NULL){
		$query->condition(db_and()->condition('e.slug', $url->slug)->condition('e.created', $start_date, '>')->condition('e.created', $end_date, '<'));
	}else{
		$query->condition('e.slug', $url->slug);
	}

	$return = $query->execute()->fetchAssoc();
	return $return['total'];
}

function _lead_track_get_total_events_user($user_id, $start_date = NULL, $end_date = NULL){
	//Use Database API to retrieve current posts.
	$query = db_select(CONST_SQL_TABLE_EVENT, 'e')
	->fields('e', array('eid'));
	$query->addExpression('COUNT(e.eid)', 'total');

	if($start_date != NULL && $end_date != NULL){
		$query->condition(db_and()->condition('e.uid', $user_id)->condition('e.created', $start_date, '>')->condition('e.created', $end_date, '<'));
	}else{
		$query->condition('e.uid', $user_id);
	}

	$return = $query->execute()->fetchAssoc();
	return $return['total'];
}

function _lead_track_get_total_visitors($start_date = NULL, $end_date = NULL){
	//Use Database API to retrieve current posts.
	$query = db_select(CONST_SQL_TABLE_VISIT, 'v')
	->fields('v', array('uid'));	
	
	if($start_date != NULL && $end_date != NULL){
		$query->condition(db_and()->condition('v.timestamp', $start_date,'>')->condition('v.timestamp', $end_date,'<'));
	}

	$query->groupBy('uid');	

	return count($query->execute()->fetchAll());
}

function _lead_track_get_total_visitors_campaign($urls, $start_date = NULL, $end_date = NULL){
	//Use Database API to retrieve current posts.
	$query = db_select(CONST_SQL_TABLE_VISIT, 'v')
	->fields('v', array('uid'));

	$u = array();

	foreach ($urls as $key => $url) {
		$u[] = $url->uid;
	}
	
	if($start_date != NULL && $end_date != NULL){
		$query->condition(db_and()->condition('v.url_id', $u, 'IN')->condition('v.timestamp', $start_date, '>')->condition('v.timestamp', $end_date, '<'));
	}else{
		$query->condition('v.url_id', $u, 'IN');
	}

	$query->groupBy('uid');	

	return count($query->execute()->fetchAll());
}

function _lead_track_get_total_visitors_url($url, $start_date = NULL, $end_date = NULL){
	//Use Database API to retrieve current posts.
	$query = db_select(CONST_SQL_TABLE_VISIT, 'v')
	->fields('v', array('uid'));

	if($start_date != NULL && $end_date != NULL){
		$query->condition(db_and()->condition('v.url_id', $url->uid)->condition('v.timestamp', $start_date, '>')->condition('v.timestamp', $end_date, '<'));
	}else{
		$query->condition('v.url_id', $url->uid);
	}

	//Group By User ID
	$query->groupBy('uid');	

	return count($query->execute()->fetchAll());
}

function _lead_track_get_total_conversions($start_date = NULL, $end_date = NULL){
	//Use Database API to retrieve current posts.
	$query = db_select(CONST_SQL_TABLE_CONVERSION, 'c')
	->fields('c', array('cid'));
	$query->addExpression('COUNT(c.cid)', 'total');

	if($start_date != NULL && $end_date != NULL){
		$query->condition(db_and()->condition('c.created', $start_date,'>')->condition('c.created', $end_date,'<'));
	}

	$return = $query->execute()->fetchAssoc();
	return $return['total'];
}

function _lead_track_get_total_conversions_campaign($urls, $start_date = NULL, $end_date = NULL){
	//Use Database API to retrieve current posts.
	$query = db_select(CONST_SQL_TABLE_CONVERSION, 'c')
	->fields('c', array('cid'));
	$query->addExpression('COUNT(c.cid)', 'total');

	$slugs = array();

	foreach ($urls as $key => $url) {
		$slugs[] = $url->slug;
	}

	if($start_date != NULL && $end_date != NULL){
		$query->condition(db_and()->condition('c.slug', $slugs, 'IN')->condition('c.created', $start_date,'>')->condition('c.created', $end_date,'<'));
	}else{
		$query->condition('c.slug', $slugs, 'IN');
	}

	$return = $query->execute()->fetchAssoc();
	return $return['total'];
}

function _lead_track_get_total_conversions_url($url, $start_date = NULL, $end_date = NULL){
	//Use Database API to retrieve current posts.
	$query = db_select(CONST_SQL_TABLE_CONVERSION, 'c')
	->fields('c', array('cid'));
	$query->addExpression('COUNT(c.cid)', 'total');

	if($start_date != NULL && $end_date != NULL){
		$query->condition(db_and()->condition('c.slug', $url->slug)->condition('c.created', $start_date,'>')->condition('c.created', $end_date,'<'));
	}else{
		$query->condition('c.slug', $url->slug);
	}

	$return = $query->execute()->fetchAssoc();
	return $return['total'];
}

function _lead_track_get_total_conversions_user($user_id, $start_date = NULL, $end_date = NULL){
	//Use Database API to retrieve current posts.
	$query = db_select(CONST_SQL_TABLE_CONVERSION, 'c')
	->fields('c', array('cid'));
	$query->addExpression('COUNT(c.cid)', 'total');

	if($start_date != NULL && $end_date != NULL){
		$query->condition(db_and()->condition('c.uid', $user_id)->condition('c.created', $start_date,'>')->condition('c.created', $end_date,'<'));
	}else{
		$query->condition('c.uid', $user_id);
	}

	$return = $query->execute()->fetchAssoc();

	return $return['total'];
}

function _lead_track_generate_url_redirection($url, $is_array = FALSE){

	$params = array();
	$url_base = '';

	if($is_array){
		$params['utm_medium'] = $url['analytics_medium'];
		$params['utm_source'] = $url['analytics_source'];
		$params['utm_content'] = $url['analytics_content'];
		$params['utm_campaign'] = $url['campaign_name'];
		$url_base = $url['url_base'];
	}else{
		$params['utm_medium'] = $url->analytics_medium;
		$params['utm_source'] = $url->analytics_source;
		$params['utm_content'] = $url->analytics_content;
		$params['utm_campaign'] = $url->campaign_name;
		$url_base = $url->url_base;
	}

	$parsed_url = parse_url($url_base);
	if(isset($parsed_url['query']) && !empty($parsed_url['query'])){

		$url = str_replace('&amp;', '&', $url_base);

		return $url . '&' . http_build_query($params); 
	}else{

		return $url_base . '?' . http_build_query($params); 

	}
	
}

function _lead_track_generate_url_admin($type, $params){

	$url = '';

	switch ($type) {
		//CAMPAIGN
		case CONST_ADMIN_URL_CAMPAIGNS:
		$url = 'admin/lead_track/campaigns';
		break;
		case CONST_ADMIN_URL_CAMPAIGNS_CAMPAIGN:
		$url = 'admin/lead_track/campaigns/'.$params['campaign_id'];
		break;
		case CONST_ADMIN_URL_CAMPAIGNS_CREATE:
		$url = 'admin/lead_track/campaigns/create';
		break;
		case CONST_ADMIN_URL_CAMPAIGNS_CAMPAIGN_EDIT:
		$url = 'admin/lead_track/campaigns/'.$params['campaign_id'].'/edit';
		break;

		//URL
		case CONST_ADMIN_URL_URLS:
		$url = 'admin/lead_track/urls';
		break;
		case CONST_ADMIN_URL_URLS_URL:
		$url = 'admin/lead_track/urls/'.$params['url_id'];
		break;
		case CONST_ADMIN_URL_URLS_CREATE:
		$url = 'admin/lead_track/urls/create';
		break;
		case CONST_ADMIN_URL_URLS_URL_EDIT:
		$url = 'admin/lead_track/urls/'.$params['url_id'].'/edit';
		break;
		
		default:
			# code...
		break;
	}

	return $url;
}

function _lead_track_translate_boolean($value, $yesNo = FALSE){
	
	$true = ($yesNo)? 'Yes' : 'True';
	$false = ($yesNo)? 'No' : 'False';
	
	return ($value)? $true : $false;

}

function _lead_track_calculate_rank_and_score($dataType, $data, $global = FALSE){

	$values = array('rank' => 0, 'score' => 0);

	return $values;

	$coef_visit = variable_get(LEAD_TRACK_V_GLOBAL_SCORE_COEF_VISIT, LEAD_TRACK_D_GLOBAL_SCORE_COEF_VISIT);
	$coef_event = variable_get(LEAD_TRACK_V_GLOBAL_SCORE_COEF_EVENT, LEAD_TRACK_D_GLOBAL_SCORE_COEF_EVENT);
	$coef_conversion = variable_get(LEAD_TRACK_V_GLOBAL_SCORE_COEF_CONVERSION, LEAD_TRACK_D_GLOBAL_SCORE_COEF_CONVERSION);

	switch ($dataType) {
		case CONST_DATA_TYPE_LEAD:
		$total_events = _lead_track_get_total_events_user($data->uid);
		$total_conversions = _lead_track_get_total_conversions_user($data->uid);
		$total_visits = _lead_track_get_total_visits_user($data->uid);

		$values['score'] += 
		$total_visits * $coef_visit 
		+ $total_events * $coef_event 
		+ $total_conversions * $coef_conversion;
		break;
		case CONST_DATA_TYPE_CAMPAIGN:

		$res = _lead_track_campaigns();

		$campaigns = $res->fetchAll();

		foreach ($campaigns as $campaign){

			//Default score
			$campaign->score = 0;

			$campaign_urls = _lead_track_urls_from_campaign($campaign->cid);

			foreach ($campaign_urls as $u){			
				$total_events = _lead_track_get_total_events_url($u);
				$total_conversions = _lead_track_get_total_conversions_url($u);
				$total_visits = _lead_track_get_total_visits_url($u);

				$campaign->score += 
				$total_visits * $coef_visit 
				+ $total_events * $coef_event 
				+ $total_conversions * $coef_conversion;					
			}
		}

		//Personal function to order campaigns by Score
		//$campaigns = (array) $campaigns;
		usort($campaigns, "orderScores");		

		//Get Position
		$index = 1;
		foreach ($campaigns as $c){			
			if($data->cid == $c->cid){
				$values['rank'] = $index;				
				$values['score'] = $c->score;
				break;
			}
			$index ++;
		}
		break;
		case CONST_DATA_TYPE_URL:

			//TODO: Calculate url rank between all the urls OR from the same campaign
		$campaign_urls = ($global)? _lead_track_urls() : _lead_track_urls_from_campaign($data->cid);

		foreach ($campaign_urls as $u){			
			$total_events = _lead_track_get_total_events_url($u);
			$total_conversions = _lead_track_get_total_conversions_url($u);
			$u->score = $u->visits*$coef_visit + $total_events*$coef_event + $total_conversions*$coef_conversion;					
		}

			//Personal function to order campaigns by count of visits
		$campaign_urls = (array) $campaign_urls;
		usort($campaign_urls, "orderScores");		

			//Get Position
		$index = 1;
		foreach ($campaign_urls as $u){			
			if($data->uid == $u->uid){
				$values['rank'] = $index;				
				$values['score'] = $u->score;
				break;
			}
			$index ++;
		}


		break;

		default:
			// code...
		break;
	}

	return $values;
}

function orderScores($current, $next)
{
	$current_value = $current->score;
	$next_value = $next->score;

	if ($current_value == $next_value) {
		return 0;
	}
	return ($current_value > $next_value) ? -1 : 1;
}

/**
* Create a tmp Tracking line into the table {CONST_SQL_TABLE_TRACKING_TMP}
*
* @param stdObject $data
* 	An object containing the visitor_id, uid, type, url, timestamp and isEntry
*
* @return boolean
* 	FALSE if error
*  TRUE if success
*
*/
function _lead_track_create_tracking_tmp($data){

	//SQL Insert function
	$sql = "insert into {".CONST_SQL_TABLE_TRACKING_TMP."}
	(visitor_id, uid, type, url, timestamp, is_entry)
	values ('$data->visitor_id', $data->uid, $data->type, '$data->url', $data->timestamp, $data->isEntry)";

	//Execute SQL query
	return db_query($sql);
}

/**
* Create an Event line into the table {CONST_SQL_TABLE_EVENT}
*
* @param stdObject $event
* 	An object containing the uid, slug, timestamp and the tracking
*
* @return boolean
* 	FALSE if error
*  TRUE if success
*
*/
function _lead_track_create_event($event){

	//SQL Insert function
	$sql = 'insert into {'.CONST_SQL_TABLE_EVENT.'}
	(`uid`, `vid`, `slug`, `created`, `tracking`)
	values ('.$event->uid.', \''.$event->visitor_id.'\', \''.$event->slug.'\', '.$event->timestamp.', :tracking)';

	//Execute SQL query
	// Use placeholder to keep {} in the string
	return db_query($sql, array(':tracking' => $event->tracking));
}

/**
* Create a Conversion line into the table {CONST_SQL_TABLE_CONVERSION}
*
* @param stdObject $conversion
* 	An object containing the uid, slug, timestamp and the tracking
*
* @return boolean
* 	FALSE if error
*  TRUE if success
*
*/
function _lead_track_create_conversion($conversion){

	//SQL Insert function
	$sql = 'insert into {'.CONST_SQL_TABLE_CONVERSION.'}
	(`uid`, `vid`, `slug`, `created`, `tracking`)
	values ('.$conversion->uid.', \''.$conversion->visitor_id.'\', \''.$conversion->slug.'\', '.$conversion->timestamp.', :tracking)';

	//Execute SQL query
	// Use placeholder to keep {} in the string
	return db_query($sql, array(':tracking' => $conversion->tracking));
}

/**
* Returns the url and its campaign from the url's slug
*
* @param string $slug
*
* @return mixed
* 	An std Object with the url and campaign info.
* 	False is returned otherwise
*/
function _lead_track_get_url_and_campaign($slug){

	//Use Database API to retrieve current posts.
	$query = db_select(CONST_SQL_TABLE_URL, 'u');
	$query->leftJoin(CONST_SQL_TABLE_CAMPAIGN, 'c', 'u.cid = c.cid');
	$query->fields('u', array('uid', 'slug', 'url_base', 'visits', 'created', 'analytics_medium', 'analytics_source', 'analytics_content', 'cid'));
	$query->fields('c', array('cid'));
	$query->addField('c', 'name', 'campaign_name');
	$query->addField('c', 'created', 'campaign_created');
	$query->addField('c', 'size', 'campaign_size');
	$query->addField('c', 'ga_event_disable', 'campaign_ga_event_disable');
	$query->addField('c', 'ga_event_value', 'campaign_ga_event_value');
	$query->addField('c', 'ga_pageview_disable', 'campaign_ga_pageview_disable');
	$query->addField('c', 'sf_enable', 'campaign_sf_enable');
	$query->addField('c', 'sf_lead_source', 'campaign_sf_lead_source');
	$query->condition('u.slug', $slug);
	$res = $query->execute();

	return $res->fetchObject();
}

function _lead_track_update_cookie($visitor_id, $status = CONST_TRACKING_COOKIE_STATUS_ACTIVE_VALUE){
	$cookieData = new stdClass();
	$cookieData->{CONST_TRACKING_COOKIE_VISITOR_KEY} = $visitor_id;
	$cookieData->{CONST_TRACKING_COOKIE_STATUS_KEY} = $status;			

			//Add visitor ID to the cookie
	$dataTracking = json_encode($cookieData);
	setrawcookie('Drupal.visitor.' . LEAD_TRACK_COOKIE_KEY_TRACKING, rawurlencode($dataTracking), REQUEST_TIME + 31536000, '/');
}

function _lead_track_get_visitor_id_from_cookie(){

	if(isset($_COOKIE['Drupal_visitor_' . LEAD_TRACK_COOKIE_KEY_TRACKING])){
		$cookieData = json_decode($_COOKIE['Drupal_visitor_' . LEAD_TRACK_COOKIE_KEY_TRACKING]);

		return (isset($cookieData->{CONST_TRACKING_COOKIE_VISITOR_KEY}))? $cookieData->{CONST_TRACKING_COOKIE_VISITOR_KEY}: NULL;
	}

	return NULL;
}

function _lead_track_set_cookie_status($status){
	if(isset($_COOKIE['Drupal_visitor_' . LEAD_TRACK_COOKIE_KEY_TRACKING])){
		$cookieData = json_decode($_COOKIE['Drupal_visitor_' . LEAD_TRACK_COOKIE_KEY_TRACKING]);

		if(isset($cookieData->{CONST_TRACKING_COOKIE_STATUS_KEY})){
			$cookieData->{CONST_TRACKING_COOKIE_STATUS_KEY} = $status;
			$dataTracking = json_encode($cookieData);
			setrawcookie('Drupal.visitor.' . LEAD_TRACK_COOKIE_KEY_TRACKING, rawurlencode($dataTracking), REQUEST_TIME + 31536000, '/');
			return TRUE;
		}
	}

	return FALSE;
}

function _lead_track_get_cookie_status(){
	if(isset($_COOKIE['Drupal_visitor_' . LEAD_TRACK_COOKIE_KEY_TRACKING])){
		$cookieData = json_decode($_COOKIE['Drupal_visitor_' . LEAD_TRACK_COOKIE_KEY_TRACKING]);

		if(isset($cookieData->{CONST_TRACKING_COOKIE_STATUS_KEY})){
			
			return $cookieData->{CONST_TRACKING_COOKIE_STATUS_KEY};
		}
	}

	return CONST_TRACKING_COOKIE_STATUS_UNACTIVE_VALUE;
}

/**
* Create a Relation line into the table {CONST_SQL_TABLE_RELATION}
*
* @param int $uid
* 	The uid of the user
*
* @param string $visitor_id
* 	The visitor_id of the user (from his cookie)
*
* @return boolean
* 	FALSE if error
*  TRUE if success
*
*/
function _lead_track_create_relation($uid, $visitor_id){

	//Check if the relation already exists in the past events and conversions
	$query = db_select(CONST_SQL_TABLE_RELATION, 'r')
	->fields('r', array('uid'));
	$query->condition('vid', $visitor_id);
	$query->isNotNull('r.uid');
	$query->range(0, 1);
	$res = $query->execute()->fetchObject();

	if($res){
		return TRUE;
	}

	//SQL Insert function
	$sql = 'insert into {' . CONST_SQL_TABLE_RELATION . '}
	(`uid`, `vid`)
	values ('.$uid.', \''.$visitor_id.'\')';

	//Execute SQL query
	return db_query($sql);
}

/**
* Returns the relation using a specific visitor_id or uid (user ID)
*
* @param string $visitor_id
*
* @param int $uid
*
* @return mixed
* 	The result of the SQL query containing the tageted tracking.
* 	False is returned otherwise
*/
function _lead_track_get_relation($visitor_id = NULL, $uid = NULL){
	//Use Database API to retrieve current posts.
	$query = db_select(CONST_SQL_TABLE_RELATION, 'r')
	->fields('r', array('rid', 'uid', 'vid'));

	($visitor_id == NULL)? $query->condition('uid', $uid): $query->condition('vid', $visitor_id);

	return $query->execute();
}

/**
* Returns all values LIKE $string for specific column $column_name
*
* @param string $column_name
*	The name of the column
*
* @param string $string
*	The pattern
*
* @return mixed
* 	The result of the SQL query.
* 	False is returned otherwise
*/
function _lead_track_url_column($column_name, $string){

	//Use Database API to retrieve current posts.
	$query = db_select(CONST_SQL_TABLE_URL, 'u')
	->fields('u', array($column_name))
	->condition('u.'.$column_name, '%' . db_like($string) . '%', 'LIKE')
	->execute();
	return $query;
}

function _lead_track_isDebug(){
	return variable_get(LEAD_TRACK_V_GLOBAL_DEBUG_ENABLE, LEAD_TRACK_D_GLOBAL_DEBUG_ENABLE);
}

/**
* Returns all the urls from the database
*
* @param null
*
* @return mixed
* 	The result of the SQL query with the campaign, ordered by the 'created' field using DESC listing.
* 	False is returned otherwise
*/
function _lead_track_urls($limit = NULL, $order_by_table = CONST_SQL_TABLE_URL, $order_by = 'visits', $ordering = 'DESC'){

	//Use Database API to retrieve data
	$query = db_select(CONST_SQL_TABLE_URL, 'u');
	$query->leftJoin(CONST_SQL_TABLE_CAMPAIGN, 'c', 'u.cid = c.cid');
	$query->fields('u', array('uid', 'slug', 'url_base', 'visits', 'created', 'analytics_medium', 'analytics_source', 'analytics_content', 'cid'));
	$query->fields('c', array('cid'));
	$query->addField('c', 'name', 'campaign_name');
	$query->addField('c', 'created', 'campaign_created');
	$query->addField('c', 'size', 'campaign_size');
	$query->addField('c', 'ga_event_disable', 'campaign_ga_event_disable');
	$query->addField('c', 'ga_event_value', 'campaign_ga_event_value');
	$query->addField('c', 'ga_pageview_disable', 'campaign_ga_pageview_disable');
	$query->addField('c', 'sf_enable', 'campaign_sf_enable');
	$query->addField('c', 'sf_lead_source', 'campaign_sf_lead_source');

	if($order_by_table == CONST_SQL_TABLE_CAMPAIGN){
		$query->orderBy('c.' . $order_by, $ordering);
	}else{
		$query->orderBy('u.' . $order_by, $ordering);
	}

	if($limit != NULL){
		$query->range(0, $limit);
	}
	
	$res = $query->execute();

	return $res->fetchAll();
}

/**
* Returns all the events for a specific uid (user ID) or in general
*
* @param int $uid
*
* @return mixed
* 	The result of the SQL query containing the tageted events.
* 	False is returned otherwise
*/
function _lead_track_get_all_events($uid = null, $limit = NULL, $order_by = 'created', $ordering = 'DESC'){
	//Use Database API to retrieve current posts.
	$query = db_select(CONST_SQL_TABLE_EVENT, 'e')
	->fields('e', array('eid', 'uid', 'vid', 'slug', 'created', 'tracking'));

	if($uid != null){
		$query->condition('uid', $uid);
	}

	$query->orderBy('e.' . $order_by, $ordering);

	if($limit != NULL){
		$query->range(0, $limit);
	}

	return $query->execute();
}

function _lead_track_get_all_events_campaign($urls, $uid = null, $limit = NULL, $order_by = 'created', $ordering = 'DESC'){
	//Use Database API to retrieve current posts.
	$query = db_select(CONST_SQL_TABLE_EVENT, 'e')
	->fields('e', array('eid', 'uid', 'vid', 'slug', 'created', 'tracking'));

	if($uid != null){
		$query->condition('uid', $uid);
	}

	$u = array();

	foreach ($urls as $key => $url) {
		$u[] = $url->slug;
	}
	
	$query->condition('e.slug', $u, 'IN');

	$query->orderBy('e.' . $order_by, $ordering);

	if($limit != NULL){
		$query->range(0, $limit);
	}

	return $query->execute();
}

function _lead_track_get_all_events_url($slug, $uid = null, $limit = NULL, $order_by = 'created', $ordering = 'DESC'){
	//Use Database API to retrieve current posts.
	$query = db_select(CONST_SQL_TABLE_EVENT, 'e')
	->fields('e', array('eid', 'uid', 'vid', 'slug', 'created', 'tracking'));

	if($uid != null){
		$query->condition('uid', $uid);
	}
	
	$query->condition('e.slug', $slug);

	$query->orderBy('e.' . $order_by, $ordering);

	if($limit != NULL){
		$query->range(0, $limit);
	}

	return $query->execute();
}

/**
* Returns all the visits for a specific uid (user ID) or in general
*
* @param int $uid
*
* @return mixed
* 	The result of the SQL query containing the tageted visits.
* 	False is returned otherwise
*/
function _lead_track_get_all_visits($uid = NULL, $limit = NULL, $order_by = 'timestamp', $ordering = 'DESC', $joinUrlData = FALSE){
	//Use Database API to retrieve current posts.
	$query = db_select(CONST_SQL_TABLE_VISIT, 'v');
	$query->fields('v', array('vid', 'uid', 'visitor_id', 'url_id', 'timestamp'));

	if($joinUrlData){
		$query->leftJoin(CONST_SQL_TABLE_URL, 'u', 'v.url_id = u.uid');	
		$query->fields('u', array('uid'));
		$query->addField('u', 'slug', 'url_slug');		
		$query->addField('u', 'created', 'url_created');
		$query->addField('u', 'url_base', 'url_url_base');
		$query->addField('u', 'visits', 'url_visits');
		$query->addField('u', 'analytics_medium', 'url_analytics_medium');
		$query->addField('u', 'analytics_source', 'url_analytics_source');
		$query->addField('u', 'analytics_content', 'url_analytics_content');
		$query->addField('u', 'cid', 'url_cid');

		$query->addExpression('COUNT(v.vid)', 'count');

		$query->groupBy('v.url_id');
	}

	if($uid != null){
		$query->condition('v.uid', $uid);
	}

	$query->orderBy('v.' . $order_by, $ordering);

	if($limit != NULL){
		$query->range(0, $limit);
	}

	$res = $query->execute();

	return $res->fetchAll();
}

function _lead_track_get_all_visits_campaign($urls, $limit = NULL, $order_by = 'timestamp', $ordering = 'DESC', $joinUrlData = FALSE){

	$query = db_select(CONST_SQL_TABLE_VISIT, 'v');
	$query->fields('v', array('vid', 'uid', 'visitor_id', 'url_id', 'timestamp'));

	if($joinUrlData){
		$query->leftJoin(CONST_SQL_TABLE_URL, 'u', 'v.url_id = u.uid');	
		$query->fields('u', array('uid'));
		$query->addField('u', 'slug', 'url_slug');		
		$query->addField('u', 'created', 'url_created');
		$query->addField('u', 'url_base', 'url_url_base');
		$query->addField('u', 'visits', 'url_visits');
		$query->addField('u', 'analytics_medium', 'url_analytics_medium');
		$query->addField('u', 'analytics_source', 'url_analytics_source');
		$query->addField('u', 'analytics_content', 'url_analytics_content');
		$query->addField('u', 'cid', 'url_cid');

		$query->addExpression('COUNT(v.vid)', 'count');

		$query->groupBy('v.url_id');
	}

	$u = array();

	foreach ($urls as $key => $url) {
		$u[] = $url->uid;
	}
	
	$query->condition('v.url_id', $u, 'IN');

	$query->orderBy('v.' . $order_by, $ordering);

	if($limit != NULL){
		$query->range(0, $limit);
	}

	$res = $query->execute();

	return $res->fetchAll();
}

function _lead_track_get_all_visits_url($url_id, $limit = NULL, $order_by = 'timestamp', $ordering = 'DESC', $joinUrlData = FALSE){
	
	$query = db_select(CONST_SQL_TABLE_VISIT, 'v');
	$query->fields('v', array('vid', 'uid', 'visitor_id', 'url_id', 'timestamp'));

	if($joinUrlData){
		$query->leftJoin(CONST_SQL_TABLE_URL, 'u', 'v.url_id = u.uid');	
		$query->fields('u', array('uid'));
		$query->addField('u', 'slug', 'url_slug');		
		$query->addField('u', 'created', 'url_created');
		$query->addField('u', 'url_base', 'url_url_base');
		$query->addField('u', 'visits', 'url_visits');
		$query->addField('u', 'analytics_medium', 'url_analytics_medium');
		$query->addField('u', 'analytics_source', 'url_analytics_source');
		$query->addField('u', 'analytics_content', 'url_analytics_content');
		$query->addField('u', 'cid', 'url_cid');

		$query->addExpression('COUNT(v.vid)', 'count');

		$query->groupBy('v.url_id');
	}	
	
	$query->condition('v.url_id', $url_id);

	$query->orderBy('v.' . $order_by, $ordering);

	if($limit != NULL){
		$query->range(0, $limit);
	}

	$res = $query->execute();

	return $res->fetchAll();
}

/**
* Returns all the visits for a specific uid (user ID) or in general
*
* @param int $uid
*
* @return mixed
* 	The result of the SQL query containing the tageted visits.
* 	False is returned otherwise
*/
function _lead_track_get_anonymous_visits(){
	//Use Database API to retrieve current posts.
	$query = db_select(CONST_SQL_TABLE_VISIT, 'v');
	$query->fields('v', array('vid', 'uid', 'visitor_id'));
	$query->isNotNull('v.visitor_id');
	$query->isNull('v.uid');
	$query->groupBy('v.visitor_id');

	$res = $query->execute();

	return $res->fetchAll();
}

/**
* Returns all the conversions for a specific uid (user ID) or in general
*
* @param int $uid
*
* @return mixed
* 	The result of the SQL query containing the tageted conversions.
* 	False is returned otherwise
*/
function _lead_track_get_all_conversions($uid = null, $limit = NULL, $order_by = 'created', $ordering = 'DESC'){
	//Use Database API to retrieve current posts.
	$query = db_select(CONST_SQL_TABLE_CONVERSION, 'c')
	->fields('c', array('cid', 'uid', 'vid', 'slug', 'created', 'tracking'));

	if($uid != null){
		$query->condition('uid', $uid);
	}

	$query->orderBy('c.' . $order_by, $ordering);

	if($limit != NULL){
		$query->range(0, $limit);
	}

	return $query->execute();
}

function _lead_track_get_all_conversions_campaign($urls, $uid = null, $limit = NULL, $order_by = 'created', $ordering = 'DESC'){
	//Use Database API to retrieve current posts.
	$query = db_select(CONST_SQL_TABLE_CONVERSION, 'c')
	->fields('c', array('cid', 'uid', 'vid', 'slug', 'created', 'tracking'));

	if($uid != null){
		$query->condition('uid', $uid);
	}

	$u = array();

	foreach ($urls as $key => $url) {
		$u[] = $url->slug;
	}
	
	$query->condition('c.slug', $u, 'IN');

	$query->orderBy('c.' . $order_by, $ordering);

	if($limit != NULL){
		$query->range(0, $limit);
	}

	return $query->execute();
}

function _lead_track_get_all_conversions_url($slug, $uid = null, $limit = NULL, $order_by = 'created', $ordering = 'DESC'){
	//Use Database API to retrieve current posts.
	$query = db_select(CONST_SQL_TABLE_CONVERSION, 'c')
	->fields('c', array('cid', 'uid', 'vid', 'slug', 'created', 'tracking'));

	if($uid != null){
		$query->condition('uid', $uid);
	}

	$query->condition('c.slug', $slug);

	$query->orderBy('c.' . $order_by, $ordering);

	if($limit != NULL){
		$query->range(0, $limit);
	}

	return $query->execute();
}

/**
* Returns the top visitors in general
*
* @param int $limit
*
* @param boolean $with_anonymous
*
* @return mixed
* 	An array containing the top visitors.
* 	False is returned otherwise
*/
function _lead_track_get_top_visitors($limit = NULL, $with_anonymous = FALSE, $ordering = 'DESC'){
	//Use Database API to retrieve current posts.
	$query = db_select(CONST_SQL_TABLE_VISIT, 'v')
	->fields('v', array('uid'));

	$query->addExpression('COUNT(v.vid)', 'total_visits');

	if(!$with_anonymous){
		$query->isNotNull('v.uid');
	}

	if($limit != NULL){
		$query->range(0, $limit);
	}

	$query->groupBy('v.uid');
	$query->orderBy('total_visits', $ordering);

	$res = $query->execute();

	return $res->fetchAll();
}

function _lead_track_get_top_visitors_campaign($urls, $limit = NULL, $with_anonymous = FALSE, $ordering = 'DESC'){
	//Use Database API to retrieve current posts.
	$query = db_select(CONST_SQL_TABLE_VISIT, 'v')
	->fields('v', array('uid'));

	$query->addExpression('COUNT(v.vid)', 'total_visits');

	if(!$with_anonymous){
		$query->isNotNull('v.uid');
	}

	if($limit != NULL){
		$query->range(0, $limit);
	}

	$u = array();

	foreach ($urls as $key => $url) {
		$u[] = $url->uid;
	}
	
	$query->condition('v.url_id', $u, 'IN');

	$query->groupBy('v.uid');
	$query->orderBy('total_visits', $ordering);

	$res = $query->execute();

	return $res->fetchAll();
}

function _lead_track_get_top_visitors_url($url_id, $limit = NULL, $with_anonymous = FALSE, $ordering = 'DESC'){
	//Use Database API to retrieve current posts.
	$query = db_select(CONST_SQL_TABLE_VISIT, 'v')
	->fields('v', array('uid'));

	$query->addExpression('COUNT(v.vid)', 'total_visits');

	if(!$with_anonymous){
		$query->isNotNull('v.uid');
	}

	if($limit != NULL){
		$query->range(0, $limit);
	}	
	
	$query->condition('v.url_id', $url_id);

	$query->groupBy('v.uid');
	$query->orderBy('total_visits', $ordering);

	$res = $query->execute();

	return $res->fetchAll();
}

/**
* Returns the top urls by events in general
*
* @param int $limit
*
* @param boolean $with_anonymous
*
* @return mixed
* 	An array containing the top visitors.
* 	False is returned otherwise
*/
function _lead_track_get_top_urls_by_events($limit = NULL, $ordering = 'DESC'){
	//Use Database API to retrieve current posts.
	$query = db_select(CONST_SQL_TABLE_EVENT, 'e')
	->fields('e', array('slug'));

	$query->addExpression('COUNT(e.eid)', 'total_events');

	if($limit != NULL){
		$query->range(0, $limit);
	}

	$query->groupBy('e.slug');
	$query->orderBy('total_events', $ordering);

	$res = $query->execute();

	return $res->fetchAll();
}

function _lead_track_get_top_urls_by_events_for_campaign($urls, $limit = NULL, $ordering = 'DESC'){
	//Use Database API to retrieve current posts.
	$query = db_select(CONST_SQL_TABLE_EVENT, 'e')
	->fields('e', array('slug'));

	$query->addExpression('COUNT(e.eid)', 'total_events');

	if($limit != NULL){
		$query->range(0, $limit);
	}

	$u = array();

	foreach ($urls as $key => $url) {
		$u[] = $url->slug;
	}
	
	$query->condition('e.slug', $u, 'IN');

	$query->groupBy('e.slug');
	$query->orderBy('total_events', $ordering);

	$res = $query->execute();

	return $res->fetchAll();
}

/**
* Returns the top urls by conversions in general
*
* @param int $limit
*
* @param boolean $with_anonymous
*
* @return mixed
* 	An array containing the top visitors.
* 	False is returned otherwise
*/
function _lead_track_get_top_urls_by_conversions($limit = NULL, $ordering = 'DESC'){
	//Use Database API to retrieve current posts.
	$query = db_select(CONST_SQL_TABLE_CONVERSION, 'c')
	->fields('c', array('slug'));

	$query->addExpression('COUNT(c.cid)', 'total_conversions');

	if($limit != NULL){
		$query->range(0, $limit);
	}

	$query->groupBy('c.slug');
	$query->orderBy('total_conversions', $ordering);

	$res = $query->execute();

	return $res->fetchAll();
}

function _lead_track_get_top_urls_by_conversions_for_campaign($urls, $limit = NULL, $ordering = 'DESC'){
	//Use Database API to retrieve current posts.
	$query = db_select(CONST_SQL_TABLE_CONVERSION, 'c')
	->fields('c', array('slug'));

	$query->addExpression('COUNT(c.cid)', 'total_conversions');

	if($limit != NULL){
		$query->range(0, $limit);
	}

	$u = array();

	foreach ($urls as $key => $url) {
		$u[] = $url->slug;
	}
	
	$query->condition('c.slug', $u, 'IN');

	$query->groupBy('c.slug');
	$query->orderBy('total_conversions', $ordering);

	$res = $query->execute();

	return $res->fetchAll();
}

/**
* Returns the tracking using a specific visitor_id or uid (user ID)
*
* @param string $visitor_id
*
* @return mixed
* 	The result of the SQL query containing the tageted tracking.
* 	False is returned otherwise
*/
function _lead_track_tracking_tmp($visitor_id = NULL, $uid = NULL){
	//Use Database API to retrieve current posts.
	$query = db_select(CONST_SQL_TABLE_TRACKING_TMP, 't')
	->fields('t', array('visitor_id', 'uid', 'type', 'url', 'timestamp', 'is_entry'));

	($visitor_id == NULL)? $query->condition('uid', $uid): $query->condition('visitor_id', $visitor_id);

	return $query->execute();
}

/**
* Returns all the tracking using a specific visitor_id and uid (user ID)
*
* @param string $visitor_id
*
* @param int $uid
*
* @return mixed
* 	The result of the SQL query containing the tageted tracking line.
* 	False is returned otherwise
*/
function _lead_track_get_all_tracking_tmp($visitor_id, $uid){
	//Use Database API to retrieve current posts.
	$query = db_select(CONST_SQL_TABLE_TRACKING_TMP, 't')
	->fields('t', array('visitor_id', 'uid', 'type', 'url', 'timestamp', 'is_entry'));

	if($uid != NULL && $uid != 0 && $visitor_id == NULL){
		$query->condition('uid', $uid);

	}else if($visitor_id != NULL && ($uid == NULL OR $uid == 0)){
		$query->condition('visitor_id', $visitor_id);

	}elseif($visitor_id == NULL && ($uid == NULL OR $uid == 0)){
		//wrong data - both empty
		return FALSE;

	}else{
		//no one is null
		$query->condition(db_or()->condition('visitor_id', $visitor_id)->condition('uid', $uid));
	}

	$query->orderBy('t.timestamp', 'ASC');

	return $query->execute();
}

/**
* Delete all the tracking for a specific visitor_id and uid (user ID)
*
* @param string $visitor_id
*
* @param int $uid
*
* @return int
* 	The number of deleted lines
*/
function _lead_track_delete_all_tracking_tmp($visitor_id, $uid){
	//Use Database API to retrieve current posts.

	$query = db_delete(CONST_SQL_TABLE_TRACKING_TMP);

	if($uid != NULL && $uid != 0 && $visitor_id == NULL){
		$query->condition('uid', $uid);

	}else if($visitor_id != NULL && ($uid == NULL OR $uid == 0)){
		$query->condition('visitor_id', $visitor_id);

	}elseif($visitor_id == NULL && ($uid == NULL OR $uid == 0)){
		//wrong data - both empty
		return FALSE;

	}else{
		//no one is null
		$query->condition(db_or()->condition('visitor_id', $visitor_id)->condition('uid', $uid));
	}

	return $query->execute();
}

/**
* Returns uid (user ID) of a visitor_id
*
* @param string $visitor_id
*
* @return mixed
* 	The uid if found.
* 	NULL is returned otherwise
*/
function _lead_track_retrieve_uid($visitor_id){

	//Check in the relation table
	$query = db_select(CONST_SQL_TABLE_RELATION, 'r')
	->fields('r', array('uid'));
	$query->condition('vid', $visitor_id);
	$query->isNotNull('r.uid');
	$query->range(0, 1);
	$res = $query->execute()->fetchObject();

	if($res){
		return $res->uid;
	}

	//Check in the past visits
	$query = db_select(CONST_SQL_TABLE_VISIT, 'v')
	->fields('v', array('uid'));
	$query->condition('visitor_id', $visitor_id);
	$query->isNotNull('v.uid');
	$query->range(0, 1);
	$res = $query->execute()->fetchObject();

	if($res){
		return $res->uid;
	}

	// If no result, check in the current tracking
	$query = db_select(CONST_SQL_TABLE_TRACKING_TMP, 't')
	->fields('t', array('uid'));
	$query->condition('visitor_id', $visitor_id);
	$query->isNotNull('t.uid');
	$query->range(0, 1);
	$res = $query->execute()->fetchObject();

	return ($res)? $res->uid: NULL;
}

/**
* Returns visitorID (COOKIE) of a user
*
* @param string $uid
*
* @return mixed
* 	The uid if found.
* 	NULL is returned otherwise
*/
function _lead_track_retrieve_visitor_id($uid){

	//Check in the past events and conversions using the relation table
	$query = db_select(CONST_SQL_TABLE_RELATION, 'r')
	->fields('r', array('vid'));
	$query->condition('uid', $uid);
	$query->isNotNull('r.vid');
	$query->range(0, 1);
	$res = $query->execute()->fetchObject();

	return ($res)? $res->vid: NULL;
}

/**
* Returns visitorID (COOKIE) of a user
*
* @param string $uid
*
* @return mixed
* 	The uid if found.
* 	NULL is returned otherwise
*/
function _lead_track_isTracked($uid){

	// Check in the current tracking
	$query = db_select(CONST_SQL_TABLE_TRACKING_TMP, 't')
	->fields('t', array('visitor_id'));
	$query->condition('uid', $uid);
	$query->isNotNull('t.visitor_id');
	$query->range(0, 1);
	$res = $query->execute()->fetchObject();

	return ($res)? $res->visitor_id: FALSE;
}

/**
* Returns TRUE is the visitor_id already exists in the table visit, tacking_tmp or relation. If not, FALSE is returned.
*
* @param string $visitor_id
*
* @return boolean
* 	TRUE if found.
* 	FALSE if not
*/
function _lead_track_visitor_id_exists($visitor_id){

	// Check in the current tracking
	$query = db_select(CONST_SQL_TABLE_TRACKING_TMP, 't')
	->fields('t', array('visitor_id'));
	$query->condition('visitor_id', $visitor_id);
	$query->range(0, 1);
	$res = $query->execute()->fetchObject();

	if($res){
		return TRUE;
	}

	// Check in the previous visits
	$query = db_select(CONST_SQL_TABLE_VISIT, 'v')
	->fields('v', array('visitor_id'));
	$query->condition('visitor_id', $visitor_id);
	$query->range(0, 1);
	$res = $query->execute()->fetchObject();

	if($res){
		return TRUE;
	}

	// Check in relations
	$query = db_select(CONST_SQL_TABLE_RELATION, 'r')
	->fields('r', array('vid'));
	$query->condition('vid', $visitor_id);
	$query->range(0, 1);
	$res = $query->execute()->fetchObject();

	if($res){
		return TRUE;
	}

	return FALSE;
}

/**
* Returns the url from an unique campaign defined by its id
*
* @param int $id_campaign
*
* @return mixed
* 	The result of the SQL query with the campaign info, ordered by the url 'created' field using DESC listing.
* 	False is returned otherwise
*/
function _lead_track_urls_from_campaign($id_campaign, $limit = 0, $order_by_table = CONST_SQL_TABLE_URL, $order_by = 'visits', $ordering = 'DESC'){

	//Use Database API to retrieve current posts.
	$query = db_select(CONST_SQL_TABLE_URL, 'u');
	$query->leftJoin(CONST_SQL_TABLE_CAMPAIGN, 'c', 'u.cid = c.cid');
	$query->fields('u', array('uid', 'slug', 'url_base', 'visits', 'created', 'analytics_medium', 'analytics_source', 'analytics_content', 'cid'));
	$query->fields('c', array('cid'));
	$query->addField('c', 'name', 'campaign_name');
	$query->addField('c', 'created', 'campaign_created');
	$query->addField('c', 'size', 'campaign_size');
	$query->addField('c', 'ga_event_disable', 'campaign_ga_event_disable');
	$query->addField('c', 'ga_event_value', 'campaign_ga_event_value');
	$query->addField('c', 'ga_pageview_disable', 'campaign_ga_pageview_disable');
	$query->addField('c', 'sf_enable', 'campaign_sf_enable');
	$query->addField('c', 'sf_lead_source', 'campaign_sf_lead_source');
	$query->condition('c.cid', $id_campaign);
	$query->orderBy('u.created', 'DESC');

	if($order_by_table == CONST_SQL_TABLE_CAMPAIGN){
		$query->orderBy('c.' . $order_by, $ordering);
	}else{
		$query->orderBy('u.' . $order_by, $ordering);
	}

	if($limit > 0){
		$query->range(0, $limit);
	}

	$res = $query->execute();

	return $res->fetchAll();
}

/**
* Returns the url using a specific slug
*
* @param string $slug
*
* @return mixed
* 	The result of the SQL query containing the tageted url.
* 	False is returned otherwise
*/
function _lead_track_url($slug){

	//Use Database API to retrieve current posts.
	$query = db_select(CONST_SQL_TABLE_URL, 'u')
	->fields('u', array('uid', 'slug', 'url_base', 'visits', 'created', 'analytics_medium', 'analytics_source', 'analytics_content', 'cid'))
	->condition('slug', $slug)
	->execute();
	return $query;
}

/**
* Returns the url using a its id
*
* @param int $id
*
* @return mixed
* 	The result of the SQL query containing the tageted url.
* 	False is returned otherwise
*/
function _lead_track_url_from_id($id){

	//Use Database API to retrieve current posts.
	$query = db_select(CONST_SQL_TABLE_URL, 'u')
	->fields('u', array('uid', 'slug', 'url_base', 'visits', 'created', 'analytics_medium', 'analytics_source', 'analytics_content', 'cid'))
	->condition('uid', $id)
	->execute();

	return $query->fetchObject();
}

/**
* Returns True is url exists
*
* @param array $params
*
* @return mixed
* 	True is url exists.
* 	False is returned otherwise
*/
function _lead_track_url_exists($params){
	//Use Database API to retrieve current posts.
	$query = db_select(CONST_SQL_TABLE_URL, 'u');
	$query->leftJoin(CONST_SQL_TABLE_CAMPAIGN, 'c', 'u.cid = c.cid');
	$query->fields('u', array('uid', 'slug', 'url_base', 'visits', 'created', 'analytics_medium', 'analytics_source', 'analytics_content', 'cid'));
	$query->fields('c', array('cid'));
	$query->addField('c', 'name', 'campaign_name');

	$query->condition(db_and()
		->condition('u.analytics_medium', $params['analytics_medium'])
		->condition('u.analytics_source', $params['analytics_source'])
		->condition('u.analytics_content', $params['analytics_content'])
		->condition('u.url_base', $params['url_base'])
		->condition('c.name', $params['campaign_name']));	

	$res = $query->execute()->fetchAll();

	return (!empty($res));
}

/**
* Generate an unique slug for a short url using ['A-Z-a-z-0-9']
*
* @param null
*
* @return string
* 	A specific length slug defined by CONST_SLUG_LENGTH
*
* @see CONST_SLUG_LENGTH
*/
function _lead_track_generate_slug(){

	//Genereate a first slug
	do{
		//generate the slug
		$chars = array_merge(range('A', 'Z'), range('a', 'z'), range(0, 9));
		shuffle($chars);
		$slug = implode(array_map(function($key) use ($chars) {
			return $chars[$key];
		}, array_rand($chars, CONST_SLUG_LENGTH)));

		//check if the slug already exists
		$res = _lead_track_url($slug);
		$result = $res->fetchAssoc();

	}while($result > 0);
	//repeat the operation until no iteration of the slug is found in the database

	return $slug;
}

/**
* Generate an unique visitor ID for a visitor using ['A-Z-a-z-0-9']
*
* @param null
*
* @return string
* 	A specific length visitor ID defined by CONST_VISITOR_ID_LENGTH
*
* @see CONST_VISITOR_ID_LENGTH
*/
function _lead_track_generate_visitorID(){

	//Genereate a first slug
	do{
		//generate the slug
		$chars = array_merge(range('A', 'Z'), range('a', 'z'), range(0, 9));
		shuffle($chars);
		$visitor_id = implode(array_map(function($key) use ($chars) {
			return $chars[$key];
		}, array_rand($chars, CONST_VISITOR_ID_LENGTH)));

		

		/*//check if the slug already exists
		$res = _lead_track_tracking_tmp($visitor_id);
		$result = $res->fetchAssoc();

		if($result <= 0){
			$result = _lead_track_get_relation($visitor_id);
			$result = $res->fetchAssoc();
		}

		*/
		//TODO: Create function to check if visitor_id is notalready in the table for visits, ttracking_tmp or relation

	}while(_lead_track_visitor_id_exists($visitor_id));
	//repeat the operation until no iteration of the slug is found in the database

	return $visitor_id;
}



/**
* Returns the campaign using its name
*
* @param string $campaign_name
* 	The name of the campaign
*
* @return mixed
* 	The result of the SQL query containing the targeted campaign.
* 	False is returned otherwise
*/
function _lead_track_campaign($campaign_name){

	//Use Database API to retrieve current posts.
	$query = db_select(CONST_SQL_TABLE_CAMPAIGN, 'u')
	->fields('u', array('cid', 'name', 'created', 'size', 'ga_event_disable', 'ga_event_value', 'ga_pageview_disable', 'sf_enable', 'sf_lead_source'))
	->condition('name', $campaign_name)
	->execute();
	return $query;
}

/**
* Returns the campaign using its id
*
* @param int $id_campaign
* 	The id of the campaign
*
* @return mixed
* 	The result of the SQL query containing the targeted campaign.
* 	False is returned otherwise
*/
function _lead_track_campaign_by_id($id_campaign){

	//Use Database API to retrieve current posts.
	$query = db_select(CONST_SQL_TABLE_CAMPAIGN, 'u')
	->fields('u', array('cid', 'name', 'created', 'size', 'ga_event_disable', 'ga_event_value', 'ga_pageview_disable', 'sf_enable', 'sf_lead_source'))
	->condition('cid', $id_campaign)
	->execute();
	return $query;
}

/**
* Returns the total visit for a campaign using its id
*
* @param int $id_campaign
* 	The id of the campaign
*
* @return mixed
* 	The result of the SQL query containing the targeted campaign.
* 	False is returned otherwise
*/
function _lead_track_total_visit_campaign_by_id($id_campaign){

	//Use Database API to retrieve current posts.
	$query = db_select(CONST_SQL_TABLE_URL, 'u')
	->fields('u', array('cid', 'visits'));
	$query->addExpression('SUM(u.visits)', 'total_visits');
	$query->condition('cid', $id_campaign);
	return $query->execute();
}

/**
* Returns all the campaigns from the database
*
* @param null
*
* @return mixed
* 	The result of the SQL query, ordered by the 'created' field using DESC listing.
* 	False is returned otherwise
*/
function _lead_track_campaigns($orderBy = 'created', $order = 'DESC'){

	//Use Database API to retrieve current posts.
	$query = db_select(CONST_SQL_TABLE_CAMPAIGN, 'u')
	->fields('u', array('cid', 'name', 'created', 'size', 'ga_event_disable', 'ga_event_value', 'ga_pageview_disable', 'sf_enable', 'sf_lead_source'))
	//Most recent first.
	->orderBy($orderBy, $order) 
	->execute();
	return $query;
}

/**
* Increase the count of visit by 1 for the specific url
*
* @param array $url
* 	An array containing the id of the url
*
* @return mixed
* 	True if success
* 	False otherwise
*/
function _lead_track_implement_visit($url){

	// Table name no longer needs {}
	$res = db_update(CONST_SQL_TABLE_URL) 
	->fields(array(
		'visits' => $url['visits'] + 1
		))
	->condition('uid', $url['uid'], '=')
	->execute();

	return ($res == 1);
}

/**
* Create new visit in table lead_track_visit
*
* @param int $uid
* 	An int containing the user ID of the visitor. NULL if anonymous.
*
* @param int $url_id
* 	An int containing the ID of the short url
*
* @return mixed
* 	True if success
* 	False otherwise
*/
function _lead_track_create_visit($uid, $visitor_id, $url_id){

	$res = db_insert(CONST_SQL_TABLE_VISIT)
	->fields(array(
		'uid' => $uid,
		'visitor_id' => $visitor_id,
		'url_id' => $url_id,
		'timestamp' => time(),
		))
	->execute();

	// //SQL Insert function
	// $sql = "insert into {".CONST_SQL_TABLE_VISIT."}
	// (uid, visitor_id, url_id, timestamp)
	// values ($uid, $visitor_id, $url_id, '$timestamp')";

	// //Execute SQL query
	// $res = db_query($sql);

	return ($res);
}

/**
* Create new cookie to track user visits until event
*
* @param int $user_id
* 	The uid of the current user
*
* @param array $url
* 	The short url
*
* @return mixed
* 	True if success
* 	False otherwise
*/
function _lead_track_start_tracking_visitor($user_id, $url){

	$user_type = ($user_id > 0) ? $user_id : 'NULL';

	$tmp_line = new stdClass();

	$visitor_id = NULL;

	if($user_id > 0){
		//Try to retrieve the visitor_id for this user
		$visitor_id = _lead_track_retrieve_visitor_id($user_id);
	}	
	
	//Generate unique visitor ID if visitor_id is null
	$tmp_line->visitor_id = ($visitor_id != NULL)? $visitor_id : _lead_track_generate_visitorID();
	$tmp_line->type = CONST_TRACKING_TYPE_SHORT_URL;
	$tmp_line->isEntry = 1;
	$tmp_line->timestamp = time();
	$tmp_line->url = $url;
	$tmp_line->uid = $user_type;	

	//Add line in tmp table
	if(!_lead_track_create_tracking_tmp($tmp_line)){

		watchdog('Lead Track - Tracking Entry', t('FAILED Tracking Entry for the short url @slug with user @uid.', array('@slug' => $tmp_line->url,'@uid' => $tmp_line->uid)), array(), WATCHDOG_ALERT);
	}elseif(_lead_track_isDebug()){
		watchdog('Lead Track - Tracking Entry', t('SUCCESS Tracking Entry for the short url @slug with user @uid.', array('@slug' => $tmp_line->url,'@uid' => $tmp_line->uid)), array(), WATCHDOG_DEBUG);
	}

	_lead_track_update_cookie($tmp_line->visitor_id, $status = CONST_TRACKING_COOKIE_STATUS_ACTIVE_VALUE);

	return $tmp_line->visitor_id;
}

/**
* Create new cookie to track user visits until event
*
* @param int $user_id
* 	The uid of the current user
*
* @param array $url
* 	The short url
*
* @return mixed
* 	True if success
* 	False otherwise
*/
function _lead_track_restart_tracking_visitor($visitor_id, $user_id, $url){

	$user_type = ($user_id > 0) ? $user_id : 'NULL';

	$tmp_line = new stdClass();

	$tmp_line->visitor_id = $visitor_id;

	$tmp_line->type = CONST_TRACKING_TYPE_SHORT_URL;
	$tmp_line->isEntry = 1;
	$tmp_line->timestamp = time();
	$tmp_line->url = $url;
	$tmp_line->uid = $user_type;

	//Add line in tmp table
	if(!_lead_track_create_tracking_tmp($tmp_line)){

		watchdog('Lead Track - Restart Tracking Entry', t('FAILED Tracking Entry for the short url @slug with user @uid.', array('@slug' => $tmp_line->url,'@uid' => $tmp_line->uid)), array(), WATCHDOG_ALERT);
	}elseif(_lead_track_isDebug()){
		watchdog('Lead Track - Restart Tracking Entry', t('SUCCESS Tracking Entry for the short url @slug with user @uid.', array('@slug' => $tmp_line->url,'@uid' => $tmp_line->uid)), array(), WATCHDOG_DEBUG);
	}

	_lead_track_update_cookie($tmp_line->visitor_id, $status = CONST_TRACKING_COOKIE_STATUS_ACTIVE_VALUE);
}