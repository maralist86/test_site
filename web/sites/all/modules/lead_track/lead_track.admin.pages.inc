<?php

/**
* Return the New Dashboard view containing all the data.
*
* @return rendering
* 	The html for the page using a specific theme
*
* @see lead_track_menu()
*/
function lead_track_new_dashboard_page(){
	
	global $base_url;

	$data = new stdClass();
	

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////
	// TOTALS
	//////////////////////////////////////////////////////////////////////////////////////////////////

	//Total Block
	$data->total = new stdClass();
	$data->total->visits = _lead_track_get_total_visits();
	$data->total->links = _lead_track_get_total_links();
	$data->total->campaigns = _lead_track_get_total_campaigns();
	$data->total->events = _lead_track_get_total_events();
	$data->total->visitors = _lead_track_get_total_visitors();
	$data->total->conversions = _lead_track_get_total_conversions();

	//TODO: For now it's for month period -- Need to be flexible for user's choice of period (day, week, year, etc)
	$data->total->js_data = new stdClass();
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
		'visitors' => '_lead_track_get_total_visitors',
		'events' => '_lead_track_get_total_events',
		'visits' => '_lead_track_get_total_visits',
		'links' => '_lead_track_get_total_links',
		'campaigns' => '_lead_track_get_total_campaigns',
		'conversions' => '_lead_track_get_total_conversions'
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
				$data->total->js_data->{$periodName} = new stdClass();
				$data->total->js_data->{$periodName}->name = date('F',mktime(0, 0, 1, $periodArray[0], 1, $year));
				continue;
			}
			$data->total->js_data->{$periodName}->{$variable} = call_user_func($function, $periodArray[1], $periodArray[2]);
		}
	}
	

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////
	// URLS
	//////////////////////////////////////////////////////////////////////////////////////////////////

	//Top URLS Block
	$data->top_urls = new stdClass();
	$data->top_urls->limit = 5;
	$top_urls_list = _lead_track_urls($data->top_urls->limit);
	foreach ($top_urls_list as $url) {
		$url->short_url = _lead_track_format_url($url->slug, urlencode($url->analytics_content));
		$url->admin_link = l($url->slug, _lead_track_generate_url_admin(CONST_ADMIN_URL_URLS_URL, $params = array('url_id' => $url->uid)));	
		$url->campaign_admin_link = l($url->campaign_name, _lead_track_generate_url_admin(CONST_ADMIN_URL_CAMPAIGNS_CAMPAIGN, $params = array('campaign_id' => $url->cid)));				
		$url->visit_percent = _lead_track_format_percentage($url->campaign_size, $url->visits);	
		$url->created = date('m/d/Y H:i:s', $url->created);
	}

	$data->top_urls->list = $top_urls_list;	

	//Recent URLS Block
	$data->all_urls = new stdClass();
	$data->all_urls->limit = 5;
	$all_urls_list = _lead_track_urls($limit = $data->all_urls->limit, $order_by_table = CONST_SQL_TABLE_URL, $order_by = 'created', $ordering = 'DESC');
	foreach ($all_urls_list as $url) {
		$url->short_url = _lead_track_format_url($url->slug, urlencode($url->analytics_content));
		$url->admin_link = l($url->slug, _lead_track_generate_url_admin(CONST_ADMIN_URL_URLS_URL, $params = array('url_id' => $url->uid)));
		$url->campaign_admin_link = l($url->campaign_name, _lead_track_generate_url_admin(CONST_ADMIN_URL_CAMPAIGNS_CAMPAIGN, $params = array('campaign_id' => $url->cid)));				
		$url->visit_percent = _lead_track_format_percentage($url->campaign_size, $url->visits);	
		$url->created = date('m/d/Y H:i:s', $url->created);
	}
	$data->all_urls->list = $all_urls_list;
	

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////
	// VISITS
	//////////////////////////////////////////////////////////////////////////////////////////////////

	//Recent Visits Block
	$data->recent_visits = new stdClass();
	$data->recent_visits->limit = 5;
	$recent_visits_list = _lead_track_get_all_visits($uid = NULL, $limit = $data->recent_visits->limit, $order_by = 'timestamp', $ordering = 'DESC');
	foreach ($recent_visits_list as $visit) {
		$visit->short_url = _lead_track_url_from_id($visit->url_id);
		$visit->short_url->admin_link = l($visit->short_url->slug, _lead_track_generate_url_admin(CONST_ADMIN_URL_URLS_URL, $params = array('url_id' => $url->uid)));
		$visit->user = ($visit->uid == NULL)? NULL : user_load($visit->uid);
		$visit->ago = nicetime($visit->timestamp);		
	}
	$data->recent_visits->list = $recent_visits_list;
	

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////
	// VISITORS
	//////////////////////////////////////////////////////////////////////////////////////////////////

	//Top Visitors Block
	$data->top_visitors = new stdClass();
	$data->top_visitors->limit = 5;
	$top_visitors_list = _lead_track_get_top_visitors($data->top_visitors->limit, $with_anonymous = TRUE);
	foreach ($top_visitors_list as $visitor) {		
		if($visitor->uid == NULL){
			$visitor->user = NULL;
			$visitor->urls = NULL;
		}else{
			$visitor->user = user_load($visitor->uid);
			$visitor->urls = _lead_track_get_all_visits($visitor->uid, $limit = NULL, $order_by = 'timestamp', $ordering = 'DESC', $joinUrlData = TRUE);
		}

	}
	$data->top_visitors->list = $top_visitors_list;


	//////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////
	// EVENTS
	//////////////////////////////////////////////////////////////////////////////////////////////////

	//Recent Events Block
	$data->recent_events = new stdClass();	
	$data->recent_events->limit = 5;
	$data->recent_events->list = array();

	//Recent Events Block -- Global data
	$data->global_events = new stdClass();
	$data->global_events->total_pages = 0;
	$data->global_events->total_time = 0;

	$res = _lead_track_get_all_events($uid = null, $limit = $data->recent_events->limit, $order_by = 'created', $ordering = 'DESC');
	if($res){
		while ($event = $res->fetchObject()) {

			$event->tracking = json_decode($event->tracking);

					//To avoid going through again for the Recent Events block
			$event->duration = formatSeconds($event->tracking->event->timestamp - $event->tracking->entry->timestamp);
			$event->tracking->entry->extra = _lead_track_get_url_and_campaign($event->tracking->entry->url);
			$event->tracking->entry->extra->admin_link = l($event->tracking->entry->extra->slug, _lead_track_generate_url_admin(CONST_ADMIN_URL_URLS_URL, $params = array('url_id' => $event->tracking->entry->extra->uid)));
			$event->user = ($event->uid == NULL)? NULL : user_load($event->uid);
			$event->ago = nicetime($event->created);
			$event->tracking->size = count($event->tracking->urls);
			$lastUrls = ($event->tracking->size > 5) ? array_slice($event->tracking->urls, -5, 5) : $event->tracking->urls;
			foreach ($lastUrls as $key => $visit){
				$visit->url = drupal_get_path_alias(str_replace($base_url . '/', '', $visit->url));
			}
			$event->tracking->urls = $lastUrls;
			$event->tracking->event->url = drupal_get_path_alias(str_replace($base_url . '/', '', $event->tracking->event->url));
			$data->recent_events->list[] = $event;

					//GLOBAL EVENT VALUES
					// + 2 for 1 entry and 1 event page
			$data->global_events->total_pages += count($event->tracking->urls) + 2;
			$data->global_events->total_time += $event->tracking->event->timestamp - $event->tracking->entry->timestamp;
		}					
	}

	$data->total->event_time = formatSeconds($data->global_events->total_time);

	//Top URL by EVENTS Block
	$data->top_url_by_events = new stdClass();
	$data->top_url_by_events->limit = 20;
	$data->top_url_by_events->list = _lead_track_get_top_urls_by_events($data->top_visitors->limit);
	foreach ($data->top_url_by_events->list as $key => &$url) {
		$res = _lead_track_url($url->slug);
		$url->extra = $res->fetchObject();				
		$url->admin_link = l($url->extra->slug, _lead_track_generate_url_admin(CONST_ADMIN_URL_URLS_URL, $params = array('url_id' => $url->extra->uid)));
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////
	// CONVERSIONS
	//////////////////////////////////////////////////////////////////////////////////////////////////

	//Recent Conversions Block
	$data->recent_conversions = new stdClass();	
	$data->recent_conversions->limit = 5;
	$data->recent_conversions->list = array();

	//Recent Conversions Block -- Global data
	$data->global_conversions = new stdClass();
	$data->global_conversions->total_pages = 0;
	$data->global_conversions->total_time = 0;

	$res = _lead_track_get_all_conversions($uid = null, $limit = $data->recent_conversions->limit, $order_by = 'created', $ordering = 'DESC');
	if($res){
		while ($conversion = $res->fetchObject()) {					

			$conversion->tracking = json_decode($conversion->tracking);

					//To avoid going through again for the Recent Events block
			$conversion->duration = formatSeconds($conversion->tracking->conversion->timestamp - $conversion->tracking->entry->timestamp);
			$conversion->tracking->entry->extra = _lead_track_get_url_and_campaign($conversion->tracking->entry->url);
			$conversion->tracking->entry->extra->admin_link = l($conversion->tracking->entry->extra->slug, _lead_track_generate_url_admin(CONST_ADMIN_URL_URLS_URL, $params = array('url_id' => $conversion->tracking->entry->extra->uid)));
			$conversion->user = ($conversion->uid == NULL)? NULL : user_load($conversion->uid);
			$conversion->ago = nicetime($conversion->created);
			$conversion->tracking->size = count($conversion->tracking->urls);
			$lastUrls = ($conversion->tracking->size > 5) ? array_slice($conversion->tracking->urls, -5, 5) : $conversion->tracking->urls;
			foreach ($lastUrls as $key => $visit){
				$visit->url = drupal_get_path_alias(str_replace($base_url . '/', '', $visit->url));
			}
			$conversion->tracking->urls = $lastUrls;					
			$conversion->tracking->conversion->url = drupal_get_path_alias(str_replace($base_url . '/', '', $conversion->tracking->conversion->url));
			$data->recent_conversions->list[] = $conversion;					

					//GLOBAL CONVERSION VALUES
					// + 2 for 1 entry and 1 conversion page
			$data->global_conversions->total_pages += count($conversion->tracking->urls) + 2;			
			$data->global_conversions->total_time += $conversion->tracking->conversion->timestamp - $conversion->tracking->entry->timestamp;
		}
	}
	$data->total->conversion_time = formatSeconds($data->global_conversions->total_time);

	//Top URL by CONVERSIONS Block
	$data->top_url_by_conversions = new stdClass();
	$data->top_url_by_conversions->limit = 20;
	$data->top_url_by_conversions->list = _lead_track_get_top_urls_by_conversions($data->top_visitors->limit);
	foreach ($data->top_url_by_conversions->list as $key => &$url) {
		$res = _lead_track_url($url->slug);
		$url->extra = $res->fetchObject();				
		$url->admin_link = l($url->extra->slug, _lead_track_generate_url_admin(CONST_ADMIN_URL_URLS_URL, $params = array('url_id' => $url->extra->uid)));
	}
	

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////
	// AVG
	//////////////////////////////////////////////////////////////////////////////////////////////////

	$data->avg = new stdClass();
	$data->avg->time_per_event = ($data->total->events == 0)? 0 : formatSeconds(floor($data->global_events->total_time / $data->total->events));
	$data->avg->time_per_conversion = ($data->total->conversions == 0)? 0 : formatSeconds(floor($data->global_conversions->total_time / $data->total->conversions));

	//For the Collapse Function
	drupal_add_js('/misc/form.js');
	drupal_add_js('/misc/collapse.js');

	return theme_lead_track_new_dashboard(array('data' => $data));
}

/**
* Return the form to add short links into html input.
*
* @param array $form
* 	The form which will be returned by the function
*
* @param array $form_state
*
* @return array
* 	The form implemented with the new elements
*
* @see lead_track_menu()
* @link https://api.drupal.org/api/drupal/developer!topics!forms_api_reference.html/7#elements
*/
function lead_track_form_html_converter_page($form, &$form_state){

	//Phase 1 - Campaign
	$form['lead_track_phase_1_fieldset'] = array(
		'#type' => 'fieldset', 
		'#title' => t('HTML Code'), 
		'#collapsible' => TRUE, 
		'#collapsed' => !(!isset($form_state["values"]["lead_track_phase"]) || $form_state["values"]["lead_track_phase"] == 0),
		);

	//Retrieve all the campaigns to provide a select input
	$res = _lead_track_campaigns();

	$result = $res->fetchAll();
	$campaigns = array();
	$default_campaign = 0;
	foreach ($result as $campaign){
		$campaigns[$campaign->cid] = $campaign->name;
		$default_campaign = $campaign->cid;
	}

	//Campaign (Mandatory)
	$form['lead_track_phase_1_fieldset']['lead_track_campaign'] = array(
		'#type' => 'select',
		'#title' => t('Campaign'),
		'#required' => TRUE,
		'#options' => $campaigns,
		'#default_value' => $default_campaign,
		'#description' => t('Select the campaign for your new short urls'),
		);

	//Medium (Mandatory)
	$form['lead_track_phase_1_fieldset']['lead_track_analytics_medium'] = array(
		'#type' => 'textfield',
		'#title' => t('Campaign Medium'),
		'#default_value' => t('Email'),
		'#size' => 50,
		'#maxlength' => 255,
		'#description' => t('The medium of your campaign - Enter . to list all previous entries'),
		'#required' => TRUE,
		'#autocomplete_path' => 'admin/lead_track/autocomplete/'.CONST_SQL_TABLE_URL.'/'.CONST_SQL_COLUMN_URL_MEDIUM ,
		);

	//Source (Mandatory)
	$form['lead_track_phase_1_fieldset']['lead_track_analytics_source'] = array(
		'#type' => 'textfield',
		'#title' => t('Campaign Source'),
		'#default_value' => t('loopfuse'),
		'#size' => 50,
		'#maxlength' => 255,
		'#description' => t('The source of your campaign (loopfuse, ics.com/newsletters) - Enter . to list all previous entries'),
		'#required' => TRUE,
		'#autocomplete_path' => 'admin/lead_track/autocomplete/'.CONST_SQL_TABLE_URL.'/'.CONST_SQL_COLUMN_URL_SOURCE ,
		);

	$form['lead_track_phase_1_fieldset']['lead_track_html_input'] = array(
		'#type' => 'textarea',
		'#title' => t('HTML Input'),
		'#default_value' => '<a href="http://drupal.dev/">Test</a>',
		'#size' => 100,
		'#rows' => 15,
		'#description' => t("The html code you want Lead Track to parse and replace every links by a short url."),
		'#required' => TRUE,
		);
	//Checkbox: Ignore errors 
	$form['lead_track_phase_1_fieldset']['lead_track_phase_1_checkbox'] = array(
		'#type' => 'checkbox',
		'#title' => t('Ignore errors'),
		'#size' => 100,
		'#required' => FALSE,
		);

	//Submit button
	$form['lead_track_phase_1_fieldset']['lead_track_phase_1_button'] = array(
		'#type' => 'button',
		'#value' => t('Parse HTML'),
		'#validate' => array('lead_track_form_html_converter_page_phase_1_validate'),
		);

	$form['lead_track_phase'] = array(
		'#type' => 'hidden',
		'#value' => 0,
		);

	if(isset($form_state["values"]["lead_track_links"]) && $form_state["values"]["lead_track_phase"] == 1){

		$links = $form_state["values"]["lead_track_links"];

		//Phase 2 - Input

		$form['lead_track_phase_2_fieldset'] = array(
			'#type' => 'fieldset', 
			'#title' => t('Links Found'), 
			'#collapsible' => TRUE, 
			'#collapsed' => FALSE,
			);

		$header = array('Link ID', 'HTML', 'URL', 'Analytics Content', 'Analytics Medium', 'Analytics Source');

		//Use the Drupal Api to render the form into our page
		$form['lead_track_phase_2_fieldset']['lead_track_links_table'] = array(
			'#theme' => 'table',
			'#header' => $header,
			'#rows' => array(),
			'#empty' => 'No Links Found',
			'#sticky' => TRUE,
			'#prefix' => '',
			'#suffix' => '',
			);	

		for ($i=0; $i < count($links['titles']); $i++) {

			$html = htmlspecialchars_decode($links['html'][$i]);
			$title = $links['titles'][$i];
			$medium = $form_state["values"]["lead_track_analytics_medium"];
			$source = $form_state["values"]["lead_track_analytics_source"];	
			$url = $links['urls'][$i];	
			$readOnly = $links['readOnly'][$i];
			$attr = array();
			if ($readOnly == 1) {
				$attr = array('readonly' => 'readonly');
			}

			$field_id = array(
				'#id' => 'link-' . $i . '-field_id',
				'#type' => 'textfield',
				'#attributes' => array('readonly' => 'readonly'),
				'#default_value' => $i,
				'#size' => 2,
				'#required' => FALSE,
				);
			$field_base_url = array(
				'#id' => 'link-' . $i . '-field_base_url',
				'#type' => 'textfield',
				'#attributes' => $attr,
				'#default_value' => $url,
				'#size' => 20,
				'#required' => FALSE,
				);
			$field_content = array(
				'#id' => 'link-' . $i . '-field_content',
				'#type' => 'textfield',
				'#default_value' => $title,
				'#size' => 20,
				'#required' => FALSE,
				'#autocomplete_path' => 'admin/lead_track/autocomplete/'.CONST_SQL_TABLE_URL.'/'.CONST_SQL_COLUMN_URL_CONTENT ,
				);
			$field_medium = array(
				'#id' => 'link-' . $i . '-field_medium',
				'#type' => 'textfield',
				'#default_value' => $medium,
				'#size' => 20,
				'#required' => FALSE,
				'#autocomplete_path' => 'admin/lead_track/autocomplete/'.CONST_SQL_TABLE_URL.'/'.CONST_SQL_COLUMN_URL_MEDIUM ,
				);
			$field_source = array(
				'#id' => 'link-' . $i . '-field_source',
				'#type' => 'textfield',
				'#default_value' => $source,
				'#size' => 20,
				'#required' => FALSE,
				'#autocomplete_path' => 'admin/lead_track/autocomplete/'.CONST_SQL_TABLE_URL.'/'.CONST_SQL_COLUMN_URL_SOURCE ,
				);

			$form['lead_track_phase_2_fieldset']['lead_track_links_table'][] = array(
				'field_id_'.$i => &$field_id,
				'field_base_url_'.$i => &$field_base_url,
				'field_content_'.$i => &$field_content,
				'field_medium_'.$i => &$field_medium,
				'field_source_'.$i => &$field_source,
				);			

			$form['lead_track_phase_2_fieldset']['lead_track_links_table']['#rows'][] = array(
				array('data' => &$field_id, 'align' => 'center'),
				$html,
				array('data' => &$field_base_url),
				array('data' => &$field_content),			
				array('data' => &$field_medium),		
				array('data' => &$field_source),
				);

			unset($field_id);
			unset($field_base_url);
			unset($field_content);
			unset($field_medium);
			unset($field_source);
		}

		//Store links for next step
		$form['lead_track_links'] = array(
			'#type' => 'hidden',
			'#value' => json_encode($links),
			);
		$form['lead_track_phase'] = array(
			'#type' => 'hidden',
			'#value' => $form_state["values"]["lead_track_phase"],
			);

		$table_attributes = array('id' => 'lead_track-html_converter-table', 'align' => 'center');
		//return theme('table', $header, $rows, $table_attributes);

		

		//Submit button
		$form['lead_track_phase_2_fieldset']['lead_track_phase_2_button'] = array(
			'#type' => 'button',
			'#value' => t('Generate HTML'),
			'#validate' => array('lead_track_form_html_converter_page_phase_2_validate'),
			);		
	}

	if(isset($form_state["values"]["lead_track_return"]) && $form_state["values"]["lead_track_phase"] == 2){
		//Phase 3 - Result

		$results =$form_state["values"]["lead_track_return"];

		$form['lead_track_phase_final_fieldset'] = array(
			'#type' => 'fieldset', 
			'#title' => t('Result'), 
			'#collapsible' => FALSE, 
			'#collapsed' => FALSE,
			);

		$html_to_parse = $form_state["values"]["lead_track_html_input"];		

		for ($i=0; $i < count($results['titles']); $i++) {

			$url = str_replace('?', '\?', $results['urls'][$i]);
			$html = htmlspecialchars_decode($results['html'][$i]);			
			$html_regex = str_replace('?', '\?', $html);
			$pattern = '~href=([\"\']??)' . $url . '\\1([^>]*)>' . $html_regex . '</a>~';
			$short_url = $results['short_urls'][$i];			

			$html_to_parse = preg_replace($pattern, "href=$1" . $short_url . "$1 $2>".$html."</a>", $html_to_parse);				
		}

		variable_set(LEAD_TRACK_V_HTML_CONVERTER_PREVIEW, $html_to_parse);

		$form['lead_track_phase_final_fieldset']['lead_track_html_result'] = array(
			'#type' => 'textarea',
			'#title' => t('Result'),
			'#value' => $html_to_parse,
			'#size' => 100,
			'#rows' => 15,
			'#description' => t("Your html code with the short urls."),
			);

		$preview_link = '/admin/lead_track/html_converter/preview';

		//Submit button
		$form['lead_track_phase_final_fieldset']['lead_track_preview'] = array(
			'#markup' => "<button type='button' class='button-user' onclick='location.href=\"$preview_link\"'>Preview</button>",
			);	
	}

	//Submit button
	$form['clear'] = array(
		'#type' => 'submit',
		'#value' => 'Reset',
		'#validate' => array('lead_track_form_html_converter_page_clear_data'),
		);

	return $form;

}

function lead_track_html_converter_preview(){
	return variable_get(LEAD_TRACK_V_HTML_CONVERTER_PREVIEW, 'No data available for preview');
}

/**
* TODO
*
* @param array $form
*
* @param array $form_state
* 	Where the values from the form are stored
*
* @return mixed
* 	TODO
*
* @see lead_track_form_html_converter_page()
*/
function lead_track_form_html_converter_page_clear_data($form, &$form_state) {

	$form_state['rebuild'] = TRUE;
	//Go to first step
	$form_state["values"]['lead_track_phase'] = 0;
}

function lead_track_form_html_converter_page_phase_1_validate($form, &$form_state) {

	//Get data from the form submission
	$html_to_parse = $form_state["values"]["lead_track_html_input"];

	$links = array(
		'titles' => array(),
		'urls' => array(),
		'readOnly' => array()
		);

	$regexp = "<a\s[^>]*href=([\"\']??)([^\" >]*?)\\1[^>]*>(.*)<\/a>";


	$error_messages = array();


	if(preg_match_all("/$regexp/siU", $html_to_parse, $matches, PREG_SET_ORDER)) {

		$pattern_special_chars = "&#?[a-z0-9]+;";

		foreach($matches as $match) {

			$readOnly = 0;
			//Check for relative urls
			$parsed_url = parse_url($match[2]);
			// Added check to ignore errors
			if((!isset($parsed_url['scheme']) || !isset($parsed_url['host'])) && ($form_state['values']['lead_track_phase_1_checkbox'] == 0)) {
				$error_messages[] = $match[2];
			}
			if(!isset($parsed_url['scheme']) || !isset($parsed_url['host'])) {
				$readOnly = 1;
			}

			if(empty($error_messages)){

				$links['urls'][] = $match[2];
				$links['readOnly'][] = $readOnly;
				$links['html'][] = htmlspecialchars($match[3]);
				$links['titles'][] = strtolower(trim(preg_replace("/$pattern_special_chars/i", "", strip_tags($match[3]))));
			}
			
		}
	}

	if(!empty($error_messages)){

		//Show errors

		$wrong_urls = '';

		foreach ($error_messages as $key => $url) {
			$wrong_urls .= '<li>' . $url . '</li>';
		}

		$msg = t(count($error_messages) .' relative link(s) found :'). '<ul>' . $wrong_urls . '</ul>';
		form_set_error('lead_track_html_input', $msg);			
		
		return;

	}else{

		//Go to next step
		$form_state["values"]['lead_track_phase']++;

		//Store data
		$form_state["values"]["lead_track_links"] = $links;
	}

}

function lead_track_form_html_converter_page_phase_2_validate($form, &$form_state) {

	//Get data from the form submission
	$values = $form_state["values"];

	$links = json_decode($values["lead_track_links"], TRUE);	
	
	$queue = array();
	$prev_redirections = array();
	$error_messages = array();

	for ($i=0; $i < count($links['titles']); $i++) {

		$utm_medium = $values['field_medium_'.$i];			
		$utm_source = $values['field_source_'.$i];
		$utm_content = $values['field_content_'.$i];
		$url_base = $links['urls'][$i];

		if($utm_medium == ""){
			$error_messages["field_medium_".$i] = t('Medium missing. ID: ' . $i . '.');
		} 

		if($utm_source == ""){
			$error_messages["field_source_".$i] = t('Source missing. ID: ' . $i . '.');
		} 

		if($utm_content == ""){
			$error_messages["field_content_".$i] = t('Content missing. ID: ' . $i . '.');			
		}		

			//Get data from the form submission to generate the redirection url
		$params['analytics_medium'] = $utm_medium;
		$params['analytics_source'] = $utm_source;
		$params['analytics_content'] = $utm_content;
		$params['url_base'] = $url_base;

			//Get the campaign array from database using its id from the form select input
		$res = _lead_track_campaign_by_id($values['lead_track_campaign']);
		$campaign = $res->fetchAssoc();
		$params['campaign_name'] = $campaign['name'];

			//Build the redirection url
		$url_redirection = _lead_track_generate_url_redirection($params, TRUE);		

		if(in_array($url_redirection, $prev_redirections)){
			$error_messages["field_id_".$i] = t('Duplicate Url + data (medium, source and content). ID: ' . $i . '.');
		}

		$result = _lead_track_url_exists($params);

		if($result){
				//Url exists so we display an error msg
			$error_messages["field_id_".$i] = t('Short url with same data already exists. ID: ' . $i . ' already exists.');
		}

		$prev_redirections[$i] = $url_redirection;

			//Use queue as a temporary storage to be able to check all the data
		$queue[$i]['url_base'] = $url_base;
		$queue[$i]['utm_medium'] = $utm_medium;
		$queue[$i]['utm_source'] = $utm_source;
		$queue[$i]['utm_content'] = $utm_content;
			//generate an unique slug
		$queue[$i]['slug'] = _lead_track_generate_slug();
			//get timestamp for the `created` database field
		$queue[$i]['timestamp'] = time();
			//Campaign ID
		$queue[$i]['cid'] = $campaign['cid'];						
	}

	if(!empty($error_messages)){		
		foreach ($error_messages as $element => $message) {			
			form_set_error($element, $message);			
		}
		return;
	}


	$j = 0;
	foreach ($queue as $key => $line) {

		
		$slug = $line['slug'];
		$timestamp = $line['timestamp'];
		$cid = $line['cid'];
		$utm_medium = $line['utm_medium'];
		$utm_source = $line['utm_source'];
		$utm_content = $line['utm_content'];
		$url_base = $line['url_base'];

		$url_uid = db_insert(CONST_SQL_TABLE_URL)
		->fields(array(
			'slug' => $slug,
			'url_base' => $url_base,
			'visits' => 0,
			'created' => $timestamp,
			'analytics_medium' => $utm_medium,
			'analytics_source' => $utm_source,
			'analytics_content' => $utm_content,
			'cid' => $cid,
			))
		->execute();

		if ($url_uid) {
				//The url HAS been created

			global $base_url;
			// if url in format %%url%% then no change	
			if (preg_match('/%%.*%%/', $url_base)) {
				$links['short_urls'][$j] = $url_base;
			} else {
				//format the short url using the slug and content from above
				$short_url =  _lead_track_format_url($slug, urlencode($utm_content));

				//add short url to links
				$links['short_urls'][$j] = $short_url;
			}
		} else {
				//the url HAS NOT been created

				//show an error message via Drupal Api
			form_set_error('', t('An unexpected error has occurred. The short url for ID='.$j.' has not been created.'));
			return;
		}

		$j++;
	}

	
	//Go to next step
	$form_state["values"]['lead_track_phase']++;

	//Store data	
	$form_state["values"]["lead_track_return"] = $links;
}


/**
* Return detailed view of a Campaign.
*
* @param array $campaign
* 	Campaign object or null - see campaign_id_load($id)
*
* @return html
* 	The view
*
* @see lead_track_menu()
*/
function lead_track_campaign_page($campaign){

	$campaign_name = '';

	$output = '';

	if($campaign == NULL){
		//TODO: show error - not campaign found
		$output = t('Error - No campaign found');
	}else{
		$campaign_name = $campaign->name;

		//TODO: show data about the campaign

		global $base_url;

		$data = new stdClass();
		$data->isCampaign = TRUE;

		//////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////
		// ID CARD
		//////////////////////////////////////////////////////////////////////////////////////////////////

		$data->card = $campaign;
		//Make data more human readable + add links
		$data->card->created = date('m/d/Y H:i:s', $campaign->created);
		$data->card->ga_event_disable_raw = $campaign->ga_event_disable;
		$data->card->ga_event_enable = _lead_track_translate_boolean(!$campaign->ga_event_disable, TRUE);
		$data->card->ga_event_value = $campaign->ga_event_value;
		$data->card->ga_pageview_enable = _lead_track_translate_boolean(!$campaign->ga_pageview_disable, TRUE);
		$data->card->sf_enable_raw = $campaign->sf_enable;
		$data->card->sf_enable = _lead_track_translate_boolean($campaign->sf_enable, TRUE);
		$data->card->sf_lead_source = $campaign->sf_lead_source;
		

		//////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////
		// ACTION
		//////////////////////////////////////////////////////////////////////////////////////////////////

		$data->action = new stdClass();
		$data->action->edit = l('Edit', _lead_track_generate_url_admin(CONST_ADMIN_URL_CAMPAIGNS_CAMPAIGN_EDIT, array('campaign_id' => $campaign->cid)), array('attributes' => array('class' => array('button'))));
		
		$urls = _lead_track_urls_from_campaign($campaign->cid);		

		if(!empty($urls)){			

			//////////////////////////////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////////////////////////////
			// TOTALS
			//////////////////////////////////////////////////////////////////////////////////////////////////

			//Total Block
			$data->total = new stdClass();
			$rankAndScore = _lead_track_calculate_rank_and_score(CONST_DATA_TYPE_CAMPAIGN, $campaign);
			$data->total->ranking = $rankAndScore['rank'];
			$data->total->score = $rankAndScore['score'];
			$data->total->visits = _lead_track_get_total_visits_campaign($urls);
			$data->total->links = _lead_track_get_total_links_campaign($urls);
			$data->total->campaigns = 1;
			$data->total->events = _lead_track_get_total_events_campaign($urls);
			$data->total->visitors = _lead_track_get_total_visitors_campaign($urls);
			$data->total->conversions = _lead_track_get_total_conversions_campaign($urls);

			//TODO: For now it's for month period -- Need to be flexible for user's choice of period (day, week, year, etc)
			$data->total->js_data = new stdClass();
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
				'visitors' => '_lead_track_get_total_visitors_campaign',
				'events' => '_lead_track_get_total_events_campaign',
				'campaigns' => null,			
				'visits' => '_lead_track_get_total_visits_campaign',
				'links' => '_lead_track_get_total_links_campaign',
				'conversions' => '_lead_track_get_total_conversions_campaign'
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
						$data->total->js_data->{$periodName}->name = date('F',mktime(0, 0, 1, $periodArray[0], 1, $year));

					}else if($variable == 'campaigns'){
					//TODO: For now it's for month period -- Need to be flexible for user's choice of period (day, week, year, etc)
						$data->total->js_data->{$periodName}->{$variable} = 1;

					}else{
						$data->total->js_data->{$periodName}->{$variable} = call_user_func($function, $urls, $periodArray[1], $periodArray[2]);
					}
				}
			}

			//////////////////////////////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////////////////////////////
			// URLS
			//////////////////////////////////////////////////////////////////////////////////////////////////

			//Top URLS Block
			$data->top_urls = new stdClass();
			$data->top_urls->limit = 50;
			$top_urls_list = _lead_track_urls_from_campaign($campaign->cid, $data->top_urls->limit);
			foreach ($top_urls_list as $url) {
				$url->short_url = _lead_track_format_url($url->slug, urlencode($url->analytics_content));
				$url->admin_link = l($url->slug, _lead_track_generate_url_admin(CONST_ADMIN_URL_URLS_URL, $params = array('url_id' => $url->uid)));
				$url->visit_percent = _lead_track_format_percentage($url->campaign_size, $url->visits);	
				$url->created = date('m/d/Y H:i:s', $url->created);
			}

			$data->top_urls->list = $top_urls_list;	

			//All URLS Block
			$data->all_urls = new stdClass();
			$all_urls_list = _lead_track_urls_from_campaign($campaign->cid, $limit = null, $order_by_table = CONST_SQL_TABLE_URL, $order_by = 'created', $ordering = 'DESC');
			foreach ($all_urls_list as $url) {
				$url->short_url = _lead_track_format_url($url->slug, urlencode($url->analytics_content));
				$url->admin_link = l($url->slug, _lead_track_generate_url_admin(CONST_ADMIN_URL_URLS_URL, $params = array('url_id' => $url->uid)));
				$url->visit_percent = _lead_track_format_percentage($url->campaign_size, $url->visits);	
				$url->created = date('m/d/Y H:i:s', $url->created);
			}
			$data->all_urls->list = $all_urls_list;


			//////////////////////////////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////////////////////////////
			// VISITS
			//////////////////////////////////////////////////////////////////////////////////////////////////

			//Recent Visits Block
			$data->recent_visits = new stdClass();
			$data->recent_visits->limit = 50;
			$recent_visits_list = _lead_track_get_all_visits_campaign($urls, $limit = $data->recent_visits->limit, $order_by = 'timestamp', $ordering = 'DESC');
			foreach ($recent_visits_list as $visit) {
				$visit->short_url = _lead_track_url_from_id($visit->url_id);
				$visit->short_url->admin_link = l($visit->short_url->slug, _lead_track_generate_url_admin(CONST_ADMIN_URL_URLS_URL, $params = array('url_id' => $visit->url_id)));
				$visit->user = ($visit->uid == NULL)? NULL : user_load($visit->uid);
				$visit->ago = nicetime($visit->timestamp);		
			}
			$data->recent_visits->list = $recent_visits_list;


			//////////////////////////////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////////////////////////////
			// VISITORS
			//////////////////////////////////////////////////////////////////////////////////////////////////

			//Top Visitors Block
			$data->top_visitors = new stdClass();
			$data->top_visitors->limit = 50;
			$top_visitors_list = _lead_track_get_top_visitors_campaign($urls, $data->top_visitors->limit, $with_anonymous = TRUE);
			foreach ($top_visitors_list as $visitor) {		
				if($visitor->uid == NULL){
					$visitor->user = NULL;
				}else{
					$visitor->user = user_load($visitor->uid);					
				}
			}
			$data->top_visitors->list = $top_visitors_list;


			//////////////////////////////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////////////////////////////
			// EVENTS
			//////////////////////////////////////////////////////////////////////////////////////////////////

			//Recent Events Block
			$data->recent_events = new stdClass();	
			$data->recent_events->limit = 50;
			$data->recent_events->list = array();

			//Recent Events Block -- Global data
			$data->global_events = new stdClass();
			$data->global_events->total_pages = 0;
			$data->global_events->total_time = 0;

			$res = _lead_track_get_all_events_campaign($urls, $uid = null, $limit = $data->recent_events->limit, $order_by = 'created', $ordering = 'DESC');
			if($res){
				while ($event = $res->fetchObject()) {

					$event->tracking = json_decode($event->tracking);

					//To avoid going through again for the Recent Events block
					$event->duration = formatSeconds($event->tracking->event->timestamp - $event->tracking->entry->timestamp);
					$event->tracking->entry->extra = _lead_track_get_url_and_campaign($event->tracking->entry->url);
					$event->tracking->entry->extra->admin_link = l($event->tracking->entry->extra->slug, _lead_track_generate_url_admin(CONST_ADMIN_URL_URLS_URL, $params = array('url_id' => $event->tracking->entry->extra->uid)));
					$event->user = ($event->uid == NULL)? NULL : user_load($event->uid);
					$event->ago = nicetime($event->created);
					$event->tracking->size = count($event->tracking->urls);
					$lastUrls = ($event->tracking->size > 5) ? array_slice($event->tracking->urls, -5, 5) : $event->tracking->urls;
					foreach ($lastUrls as $key => $visit){
						$visit->url = drupal_get_path_alias(str_replace($base_url . '/', '', $visit->url));
					}
					$event->tracking->urls = $lastUrls;
					$event->tracking->event->url = drupal_get_path_alias(str_replace($base_url . '/', '', $event->tracking->event->url));
					$data->recent_events->list[] = $event;

					//GLOBAL EVENT VALUES
					// + 2 for 1 entry and 1 event page
					$data->global_events->total_pages += $event->tracking->size + 2;
					$data->global_events->total_time += $event->tracking->event->timestamp - $event->tracking->entry->timestamp;
				}					
			}

			$data->total->event_time = formatSeconds($data->global_events->total_time);

			//Top URL by EVENTS Block
			$data->top_url_by_events = new stdClass();
			$data->top_url_by_events->limit = 50;
			$data->top_url_by_events->list = _lead_track_get_top_urls_by_events_for_campaign($urls, $data->top_visitors->limit);
			foreach ($data->top_url_by_events->list as $key => &$url) {
				$res = _lead_track_url($url->slug);
				$url->extra = $res->fetchObject();				
				$url->admin_link = l($url->extra->slug, _lead_track_generate_url_admin(CONST_ADMIN_URL_URLS_URL, $params = array('url_id' => $url->extra->uid)));
			}


			//////////////////////////////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////////////////////////////
			// CONVERSIONS
			//////////////////////////////////////////////////////////////////////////////////////////////////

			//Recent Conversions Block
			$data->recent_conversions = new stdClass();	
			$data->recent_conversions->limit = 50;
			$data->recent_conversions->list = array();

			//Recent Conversions Block -- Global data
			$data->global_conversions = new stdClass();
			$data->global_conversions->total_pages = 0;
			$data->global_conversions->total_time = 0;

			$res = _lead_track_get_all_conversions_campaign($urls, $uid = null, $limit = $data->recent_conversions->limit, $order_by = 'created', $ordering = 'DESC');
			if($res){
				while ($conversion = $res->fetchObject()) {					

					$conversion->tracking = json_decode($conversion->tracking);

					//To avoid going through again for the Recent Events block
					$conversion->duration = formatSeconds($conversion->tracking->conversion->timestamp - $conversion->tracking->entry->timestamp);
					$conversion->tracking->entry->extra = _lead_track_get_url_and_campaign($conversion->tracking->entry->url);
					$conversion->tracking->entry->extra->admin_link = l($conversion->tracking->entry->extra->slug, _lead_track_generate_url_admin(CONST_ADMIN_URL_URLS_URL, $params = array('url_id' => $conversion->tracking->entry->extra->uid)));
					$conversion->user = ($conversion->uid == NULL)? NULL : user_load($conversion->uid);
					$conversion->ago = nicetime($conversion->created);
					$conversion->tracking->size = count($conversion->tracking->urls);
					$lastUrls = ($conversion->tracking->size > 5) ? array_slice($conversion->tracking->urls, -5, 5) : $conversion->tracking->urls;
					foreach ($lastUrls as $key => $visit){
						$visit->url = drupal_get_path_alias(str_replace($base_url . '/', '', $visit->url));
					}
					$conversion->tracking->urls = $lastUrls;					
					$conversion->tracking->conversion->url = drupal_get_path_alias(str_replace($base_url . '/', '', $conversion->tracking->conversion->url));
					$data->recent_conversions->list[] = $conversion;					

					//GLOBAL CONVERSION VALUES
					// + 2 for 1 entry and 1 conversion page
					$data->global_conversions->total_pages += $conversion->tracking->size + 2;			
					$data->global_conversions->total_time += $conversion->tracking->conversion->timestamp - $conversion->tracking->entry->timestamp;
				}
			}
			$data->total->conversion_time = formatSeconds($data->global_conversions->total_time);

			//Top URL by CONVERSIONS Block
			$data->top_url_by_conversions = new stdClass();
			$data->top_url_by_conversions->limit = 50;
			$data->top_url_by_conversions->list = _lead_track_get_top_urls_by_conversions_for_campaign($urls, $data->top_visitors->limit);
			foreach ($data->top_url_by_conversions->list as $key => &$url) {
				$res = _lead_track_url($url->slug);
				$url->extra = $res->fetchObject();				
				$url->admin_link = l($url->extra->slug, _lead_track_generate_url_admin(CONST_ADMIN_URL_URLS_URL, $params = array('url_id' => $url->extra->uid)));
			}

			//////////////////////////////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////////////////////////////
			// AVG
			//////////////////////////////////////////////////////////////////////////////////////////////////

			$data->avg = new stdClass();
			$data->avg->time_per_event = ($data->total->events == 0)? 0 : formatSeconds(floor($data->global_events->total_time / $data->total->events));
			$data->avg->time_per_conversion = ($data->total->conversions == 0)? 0 : formatSeconds(floor($data->global_conversions->total_time / $data->total->conversions));

			//For the Collapse Function
			drupal_add_js('/misc/form.js');
			drupal_add_js('/misc/collapse.js');			
		}		

		$output = theme_lead_track_campaign(array('data' => $data));
	}

	drupal_set_title(t('Campaign - ') . $campaign_name);	

	return $output;
}

/**
* Return detailed view of a Url.
*
* @param array $campaign
* 	Url object or null - see url_id_load($id)
*
* @return html
* 	The view
*
* @see lead_track_menu()
*/
function lead_track_url_page($url){

	$url_name = '';

	$output = '';

	if($url == NULL){
		//TODO: show error - not campaign found
		$output = t('Error - No url found');
	}else{

		global $base_url;

		$url_name = $url->slug;

		$res = _lead_track_campaign_by_id($url->cid);
		$campaign = $res->fetchAssoc();
		

		//TODO: show data about the url

		$data = new stdClass();

		//////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////
		// ID CARD
		//////////////////////////////////////////////////////////////////////////////////////////////////

		$data->card = $url;
		//Make data more human readable + add links
		$data->card->created = date('m/d/Y H:i:s', $url->created);
		$data->card->campaign_link = l($campaign['name'], _lead_track_generate_url_admin(CONST_ADMIN_URL_CAMPAIGNS_CAMPAIGN, array('campaign_id' => $campaign['cid'])));
		$data->card->copyToClipBoard = '<a href="#" onclick="javascript:copyToClipboard(\''. _lead_track_format_url($url->slug, urlencode($url->analytics_content)) .'\');">'. _lead_track_format_url($url->slug, urlencode($url->analytics_content)).'</a>';


		//////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////
		// ACTION
		//////////////////////////////////////////////////////////////////////////////////////////////////

		$data->action = new stdClass();
		$data->action->edit = l('Edit', _lead_track_generate_url_admin(CONST_ADMIN_URL_URLS_URL_EDIT, array('url_id' => $url->uid)), array('attributes' => array('class' => array('button'))));
		$data->action->copy = l('Copy', _lead_track_generate_url_admin(CONST_ADMIN_URL_URLS_CREATE, NULL), array('attributes' => array('class' => array('button'))));





		//////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////
		// TOTALS
		//////////////////////////////////////////////////////////////////////////////////////////////////

		//Total Block
		$data->total = new stdClass();
		$rankAndScore = _lead_track_calculate_rank_and_score(CONST_DATA_TYPE_URL, $url);
		$data->total->ranking = $rankAndScore['rank'];
		$data->total->score = $rankAndScore['score'];
		$data->total->visits = _lead_track_get_total_visits_url($url);
		$data->total->events = _lead_track_get_total_events_url($url);
		$data->total->visitors = _lead_track_get_total_visitors_url($url);
		$data->total->conversions = _lead_track_get_total_conversions_url($url);

		//TODO: For now it's for month period -- Need to be flexible for user's choice of period (day, week, year, etc)
		$data->total->js_data = new stdClass();
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
			'visitors' => '_lead_track_get_total_visitors_url',
			'events' => '_lead_track_get_total_events_url',		
			'visits' => '_lead_track_get_total_visits_url',
			'conversions' => '_lead_track_get_total_conversions_url'
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
					$data->total->js_data->{$periodName}->name = date('F',mktime(0, 0, 1, $periodArray[0], 1, $year));

				}else{
					$data->total->js_data->{$periodName}->{$variable} = call_user_func($function, $url, $periodArray[1], $periodArray[2]);
				}
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////
		// VISITS
		//////////////////////////////////////////////////////////////////////////////////////////////////

		//Recent Visits Block
		$data->recent_visits = new stdClass();
		$data->recent_visits->limit = 50;
		$recent_visits_list = _lead_track_get_all_visits_url($url->uid, $limit = $data->recent_visits->limit, $order_by = 'timestamp', $ordering = 'DESC');
		foreach ($recent_visits_list as $visit) {
			$visit->short_url = _lead_track_url_from_id($visit->url_id);
			$visit->user = ($visit->uid == NULL)? NULL : user_load($visit->uid);
			$visit->ago = nicetime($visit->timestamp);		
		}
		$data->recent_visits->list = $recent_visits_list;


		//////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////
		// VISITORS
		//////////////////////////////////////////////////////////////////////////////////////////////////

		//Top Visitors Block
		$data->top_visitors = new stdClass();
		$data->top_visitors->limit = 50;
		$top_visitors_list = _lead_track_get_top_visitors_url($url->uid, $data->top_visitors->limit, $with_anonymous = TRUE);
		foreach ($top_visitors_list as $visitor) {		
			if($visitor->uid == NULL){
				$visitor->user = NULL;
				$visitor->urls = NULL;
			}else{
				$visitor->user = user_load($visitor->uid);
				$visitor->urls = _lead_track_get_all_visits($visitor->uid, $limit = NULL, $order_by = 'timestamp', $ordering = 'DESC', $joinUrlData = TRUE);
			}

		}
		$data->top_visitors->list = $top_visitors_list;


		//////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////
		// EVENTS
		//////////////////////////////////////////////////////////////////////////////////////////////////

		//Recent Events Block
		$data->recent_events = new stdClass();	
		$data->recent_events->limit = 50;

		$res = _lead_track_get_all_events_url($url->slug, $uid = null, $limit = null, $order_by = 'created', $ordering = 'DESC');
		if($res){
			while ($event = $res->fetchObject()) {

				$event->tracking = json_decode($event->tracking);

					//To avoid going through again for the Recent Events block
				$event->duration = formatSeconds($event->tracking->event->timestamp - $event->tracking->entry->timestamp);
				$event->tracking->entry->extra = _lead_track_get_url_and_campaign($event->tracking->entry->url);
				$event->user = ($event->uid == NULL)? NULL : user_load($event->uid);
				$event->ago = nicetime($event->created);
				$event->tracking->size = count($event->tracking->urls);
				$lastUrls = ($event->tracking->size > 5) ? array_slice($event->tracking->urls, -5, 5) : $event->tracking->urls;
				foreach ($lastUrls as $key => $visit){
					$visit->url = drupal_get_path_alias(str_replace($base_url . '/', '', $visit->url));
				}
				$event->tracking->urls = $lastUrls;
				$event->tracking->event->url = drupal_get_path_alias(str_replace($base_url . '/', '', $event->tracking->event->url));
				$data->recent_events->list[] = $event;
			}					
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////
		// CONVERSIONS
		//////////////////////////////////////////////////////////////////////////////////////////////////

		//Recent Conversions Block
		$data->recent_conversions = new stdClass();	
		$data->recent_conversions->limit = 50;

		$res = _lead_track_get_all_conversions_url($url->slug, $uid = null, $limit = null, $order_by = 'created', $ordering = 'DESC');
		if($res){
			while ($conversion = $res->fetchObject()) {					

				$conversion->tracking = json_decode($conversion->tracking);

					//To avoid going through again for the Recent Events block
				$conversion->duration = formatSeconds($conversion->tracking->conversion->timestamp - $conversion->tracking->entry->timestamp);
				$conversion->tracking->entry->extra = _lead_track_get_url_and_campaign($conversion->tracking->entry->url);
				$conversion->user = ($conversion->uid == NULL)? NULL : user_load($conversion->uid);
				$conversion->ago = nicetime($conversion->created);
				$conversion->tracking->size = count($conversion->tracking->urls);
				$lastUrls = ($conversion->tracking->size > 5) ? array_slice($conversion->tracking->urls, -5, 5) : $conversion->tracking->urls;
				foreach ($lastUrls as $key => $visit){
					$visit->url = drupal_get_path_alias(str_replace($base_url . '/', '', $visit->url));
				}
				$conversion->tracking->urls = $lastUrls;
				$conversion->tracking->conversion->url = drupal_get_path_alias(str_replace($base_url . '/', '', $conversion->tracking->conversion->url));
				$data->recent_conversions->list[] = $conversion;
			}
		}


		//For the Collapse Function
		drupal_add_js('/misc/form.js');
		drupal_add_js('/misc/collapse.js');	



		$output = theme_lead_track_url(array('data' => $data));
	}

	drupal_set_title(t('Url - ') . $url_name);	

	return $output;
}