<?php

/**
 * The Lead Track User Tool, accessible from user page under the specific tab.
 *
 * @param $stdClass $user
 * 	Use to retrieve local data from the user
 *
 * @return html
 * 	A dashboard view with all the user data
 *
 * @see lead_track_menu()
 */
function lead_track_user_tool_page($user){

	global $base_url;

	$user = user_load($user->uid, TRUE);

	$data = new stdClass();
	$data->user = new stdClass();

	foreach($user as $field => $value){

		if(is_array($value) && isset($value[LANGUAGE_NONE][0])){
			$item = $value[LANGUAGE_NONE][0];
			$value = field_view_value('user', $user, $field, $item);
		}
		$data->user->extra[$field] = $value;
	}

	$data->user->name = $user->name;
	$data->user->status = $user->status;
	$data->user->timezone = $user->timezone;
	$data->user->mail = $user->mail;
	$data->user->created = $user->created;
	$data->user->login = $user->login;
	$data->user->access = $user->access;
	$data->user->created_ago = nicetime($user->created);
	$data->user->login_ago = nicetime($user->login);
	$data->user->access_ago = nicetime($user->access);

	$data->user->analytics = new stdClass();
	// TODO: Have a generic way to define field_ga_visitor_id (maybe in the module configuration?)
	if(isset($user->field_ga_visitor_id[LANGUAGE_NONE][0]['value'])){
		$data->user->analytics->visitor_id = $user->field_ga_visitor_id[LANGUAGE_NONE][0]['value'];
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////
	// DATA FOR GRAPH
	//////////////////////////////////////////////////////////////////////////////////////////////////
	//TODO: For now it's for month period -- Need to be flexible for user's choice of period (day, week, year, etc)
	$data->user->total->js_data = new stdClass();
	$year = date('Y');

	$period_2_ago = date('m') - 2;		
	$start_2_ago = mktime(0, 0, 1, $period_2_ago, 1, $year);
	$end_2_ago = mktime(23, 59, 00, $period_2_ago, date('t', $period_2_ago), $year);


	$period_1_ago = date('m') - 1;
	$start_1_ago = mktime(0, 0, 1, $period_1_ago, 1, $year);
	$end_1_ago = mktime(23, 59, 00, $period_1_ago, date('t', $period_1_ago), $year);

	$period_current = date('m');
	$start_current = mktime(0, 0, 1, $period_current, 1, $year);
	$end_current = mktime(23, 59, 00, $period_current, date('t', $period_current), $year);

	$dataFunctions = array(
		'name' => null,
		'events' => '_lead_track_get_total_events_user',
		'visits' => '_lead_track_get_total_visits_user',
		'conversions' => '_lead_track_get_total_conversions_user'
		);

	$periods = array(
		'period_2_ago' => array($period_2_ago, $start_2_ago, $end_2_ago),
		'period_1_ago' => array($period_1_ago, $start_1_ago, $end_1_ago),
		'period_current' => array($period_current, $start_current, $end_current)
		);

	foreach ($periods as $periodName => $periodArray) {
		
		foreach ($dataFunctions as $variable => $function) {

			if($variable == 'name'){
				//TODO: For now it's for month period -- Need to be flexible for user's choice of period (day, week, year, etc)
				$data->user->total->js_data->{$periodName}->name = date('F',mktime(0, 0, 1, $periodArray[0], 1, $year));
				continue;
			}
			$data->user->total->js_data->{$periodName}->{$variable} = call_user_func($function, $user->uid, $periodArray[1], $periodArray[2]);
		}
	}
	$values = _lead_track_calculate_rank_and_score(CONST_DATA_TYPE_LEAD, $user);
	$data->user->score = $values['score'];

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////
	// EVENTS
	//////////////////////////////////////////////////////////////////////////////////////////////////
	//Retrieve events from the user
	$data->user->events = array();
	$data->user->_global_events = new stdClass();
	$data->user->_global_events->total_events = _lead_track_get_total_events_user($user->uid);
	$data->global_events->total_pages = 0;

	$res = _lead_track_get_all_events($uid = $user->uid, $limit = null, $order_by = 'created', $ordering = 'DESC');
	if($res){
		while ($event = $res->fetchObject()) {

			$event->tracking = json_decode($event->tracking);

			$event->duration = formatSeconds($event->tracking->event->timestamp - $event->tracking->entry->timestamp);
			$event->tracking->entry->extra = _lead_track_get_url_and_campaign($event->tracking->entry->url);


			$event->tracking->entry->extra->admin_link = l($event->tracking->entry->extra->slug, _lead_track_generate_url_admin(CONST_ADMIN_URL_URLS_URL, $params = array('url_id' => $event->tracking->entry->extra->uid)));
			$event->tracking->entry->extra->admin_link_campaign = l($event->tracking->entry->extra->campaign_name, _lead_track_generate_url_admin(CONST_ADMIN_URL_CAMPAIGNS_CAMPAIGN, array('campaign_id' => $event->tracking->entry->extra->cid)));

			$event->ago = nicetime($event->created);
			$event->tracking->size = count($event->tracking->urls);
			$lastUrls = ($event->tracking->size > 5) ? array_slice($event->tracking->urls, -5, 5) : $event->tracking->urls;
			foreach ($lastUrls as $key => $visit){
				$visit->url = drupal_get_path_alias(str_replace($base_url . '/', '', $visit->url));
			}
			$event->tracking->urls = $lastUrls;
			$event->tracking->event->url = drupal_get_path_alias(str_replace($base_url . '/', '', $event->tracking->event->url));

			$data->user->events[] = $event;

			//GLOBAL EVENT VALUES
			// + 2 for 1 entry and 1 conversion page
			$data->global_events->total_pages += $event->tracking->size + 2;
		}					
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////
	// CONVERSIONS
	//////////////////////////////////////////////////////////////////////////////////////////////////
	//Retrieve conversions from the user
	$data->user->conversions = array();
	$data->user->_global_conversions = new stdClass();
	$data->user->_global_conversions->total_conversions = _lead_track_get_total_conversions_user($user->uid);
	$data->global_conversions->total_pages = 0;

	$total_conversions = 0;

	$res = _lead_track_get_all_conversions($user->uid, $limit = null, $order_by = 'created', $ordering = 'DESC');
	if($res){
		while ($conversion = $res->fetchObject()) {					

			$conversion->tracking = json_decode($conversion->tracking);

			$conversion->duration = formatSeconds($conversion->tracking->conversion->timestamp - $conversion->tracking->entry->timestamp);
			$conversion->tracking->entry->extra = _lead_track_get_url_and_campaign($conversion->tracking->entry->url);
			$conversion->tracking->entry->extra->admin_link = l($conversion->tracking->entry->extra->slug, _lead_track_generate_url_admin(CONST_ADMIN_URL_URLS_URL, $params = array('url_id' => $conversion->tracking->entry->extra->uid)));
			$conversion->tracking->entry->extra->admin_link_campaign = l($conversion->tracking->entry->extra->campaign_name, _lead_track_generate_url_admin(CONST_ADMIN_URL_CAMPAIGNS_CAMPAIGN, array('campaign_id' => $conversion->tracking->entry->extra->cid)));

			$conversion->ago = nicetime($conversion->created);
			$conversion->tracking->size = count($conversion->tracking->urls);
			$lastUrls = ($conversion->tracking->size > 5) ? array_slice($conversion->tracking->urls, -5, 5) : $conversion->tracking->urls;
			foreach ($lastUrls as $key => $visit){
				$visit->url = drupal_get_path_alias(str_replace($base_url . '/', '', $visit->url));
			}
			$conversion->tracking->urls = $lastUrls;					
			$conversion->tracking->conversion->url = drupal_get_path_alias(str_replace($base_url . '/', '', $conversion->tracking->conversion->url));

			$data->user->conversions[] = $conversion;

			//GLOBAL CONVERSION VALUES
			// + 2 for 1 entry and 1 conversion page
			$data->global_conversions->total_pages += $conversion->tracking->size + 2;
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////
	// VISITS
	//////////////////////////////////////////////////////////////////////////////////////////////////
	//Retrieve visits from the user
	$data->user->visits = array();
	$data->user->_global_visits = new stdClass();
	$data->user->_global_visits->total_visits = _lead_track_get_total_visits_user($user->uid);

	$data->user->visits = _lead_track_get_all_visits($uid = $user->uid, $limit = null, $order_by = 'timestamp', $ordering = 'DESC');
	if($data->user->visits){
		foreach ($data->user->visits as $visit) {
			$visit->short_url = _lead_track_url_from_id($visit->url_id);
			$visit->short_url->admin_link = l($visit->short_url->slug, _lead_track_generate_url_admin(CONST_ADMIN_URL_URLS_URL, $params = array('url_id' => $visit->url_id)));
			$visit->ago = nicetime($visit->timestamp);
			$campaign = _lead_track_campaign_by_id($visit->short_url->cid);
			$visit->campaign = $campaign->fetchObject();
			$visit->campaign->admin_link = l($visit->campaign->name, _lead_track_generate_url_admin(CONST_ADMIN_URL_CAMPAIGNS_CAMPAIGN, array('campaign_id' => $visit->campaign->cid)));
		}					
	}

	//For the Collapse Function
	drupal_add_js('/misc/form.js');
	drupal_add_js('/misc/collapse.js');

	return theme_lead_track_user_tool(array('data' => $data));
}