<script type="text/javascript">

	function drawChartGlobal() {
		
		var data = google.visualization.arrayToDataTable([
			['Month', 'Conversions', 'Events', 'Visits', 'Visitors'],
			['<?php print $js_data->period_2_ago->name; ?>', <?php print $js_data->period_2_ago->conversions; ?>, <?php print $js_data->period_2_ago->events; ?>, <?php print $js_data->period_2_ago->visits; ?>, <?php print $js_data->period_2_ago->visitors; ?>],
			['<?php print $js_data->period_1_ago->name; ?>', <?php print $js_data->period_1_ago->conversions; ?>, <?php print $js_data->period_1_ago->events; ?>, <?php print $js_data->period_1_ago->visits; ?>, <?php print $js_data->period_1_ago->visitors; ?>],
			['<?php print $js_data->period_current->name; ?>', <?php print $js_data->period_current->conversions; ?>, <?php print $js_data->period_current->events; ?>, <?php print $js_data->period_current->visits; ?>, <?php print $js_data->period_current->visitors; ?>]
			]);

		var options = {
			title: '3 last months',
			legend: {
				position: 'bottom'
			},
			chartArea: {left:'2%', right: '8%', width:'90%'},
			vAxes: {
				1: {
					logScale: false,
					minValue: 0,
					format:'#',
					textStyle: {color: 'black'}

				}
			},
			seriesType:"bars",
			series:{
				0:{
					type:"line", 
					targetAxisIndex: 0,
					color: CONFIG.color.conversion,
					targetAxisIndex: 1
				}, 
				1:{
					type:"line", 
					targetAxisIndex: 0,
					color: CONFIG.color.event,
					targetAxisIndex: 1
				},
				2:{
					type:"bar", 
					targetAxisIndex: 0,
					color: CONFIG.color.visit,
					targetAxisIndex: 1
				},
				3:{
					type:"bar", 
					targetAxisIndex: 0,
					color: CONFIG.color.visitor,
					targetAxisIndex: 1
				}
			},
			isStacked: true

		};

		var chart = new google.visualization.ColumnChart(document.getElementById('comboView-global'));
		chart.draw(data, options);                             
	}

	function drawChartVisits() {
		var data = google.visualization.arrayToDataTable([
			['Month', 'Visitors', 'Visits'],
			['<?php print $js_data->period_2_ago->name; ?>', <?php print $js_data->period_2_ago->visitors; ?>, <?php print $js_data->period_2_ago->visits; ?>],
			['<?php print $js_data->period_1_ago->name; ?>', <?php print $js_data->period_1_ago->visitors; ?>, <?php print $js_data->period_1_ago->visits; ?>],
			['<?php print $js_data->period_current->name; ?>', <?php print $js_data->period_current->visitors; ?>, <?php print $js_data->period_current->visits; ?>]
			]);

		var options = {
			title: 'Visits - 3 last months',				
			legend: {
				position: 'right'
			},
			chartArea: {left:'5%', right: '10%', width:'85%'},
			vAxes: {
				0: {
					logScale: false,
					minValue: 0,
					format:'#'
				}
			},
			series:{
				0:{ 
					targetAxisIndex: 0,
					color: CONFIG.color.visitor
				}, 
				1:{
					targetAxisIndex: 0,
					color: CONFIG.color.visit
				}
			}
		};			

		var chart = new google.visualization.LineChart(document.getElementById('lineView-visits'));
		chart.draw(data, options);

		isVisitsGraphShown = true;	
	}

	function drawChartUrls() {
		var data = google.visualization.arrayToDataTable([
			['Month', 'Urls', 'Campaigns'],
			['<?php print $js_data->period_2_ago->name; ?>', <?php print $js_data->period_2_ago->links; ?>, <?php print $js_data->period_2_ago->campaigns; ?>],
			['<?php print $js_data->period_1_ago->name; ?>', <?php print $js_data->period_1_ago->links; ?>, <?php print $js_data->period_1_ago->campaigns; ?>],
			['<?php print $js_data->period_current->name; ?>', <?php print $js_data->period_current->links; ?>, <?php print $js_data->period_current->campaigns; ?>]
			]);

		var options = {
			title: 'Visits & Campaigns',
			legend: {
				position: 'right'
			},
			chartArea: {left:'5%', right: '10%', width:'85%'},
			vAxes: {
				0: {
					logScale: false,
					minValue: 0,
					format:'#'
				}
			},
			series:{
				0:{ 
					targetAxisIndex: 0,
					color: CONFIG.color.url
				}, 
				1:{
					targetAxisIndex: 0,
					color: CONFIG.color.campaign
				}
			}
		};

		var chart = new google.visualization.LineChart(document.getElementById('lineView-urls'));
		chart.draw(data, options);

		isUrlsGraphShown = true;
	}

	function drawChartEvents() {
		var data = google.visualization.arrayToDataTable([
			['Month', 'Events'],
			['<?php print $js_data->period_2_ago->name; ?>', <?php print $js_data->period_2_ago->events; ?>],
			['<?php print $js_data->period_1_ago->name; ?>', <?php print $js_data->period_1_ago->events; ?>],
			['<?php print $js_data->period_current->name; ?>', <?php print $js_data->period_current->events; ?>]
			]);

		var options = {
			title: 'Events',
			legend: {
				position: 'right'
			},
			chartArea: {left:'5%', right: '10%', width:'85%'},
			vAxes: {
				0: {
					logScale: false,
					minValue: 0,
					format:'#'
				}
			},
			series:{
				0:{ 
					targetAxisIndex: 0,
					color: CONFIG.color.event
				}
			}
		};

		var chart = new google.visualization.LineChart(document.getElementById('lineView-events'));
		chart.draw(data, options);

		isEventsGraphShown = true;
	}

	function drawChartConversions() {
		var data = google.visualization.arrayToDataTable([
			['Month', 'Conversions'],
			['<?php print $js_data->period_2_ago->name; ?>', <?php print $js_data->period_2_ago->conversions; ?>],
			['<?php print $js_data->period_1_ago->name; ?>', <?php print $js_data->period_1_ago->conversions; ?>],
			['<?php print $js_data->period_current->name; ?>', <?php print $js_data->period_current->conversions; ?>]
			]);

		var options = {
			title: 'Conversions',
			legend: {
				position: 'right'
			},
			chartArea: {left:'5%', right: '10%', width:'85%'},
			vAxes: {
				0: {
					logScale: false,
					minValue: 0,
					format:'#'
				}
			},
			series:{
				0:{ 
					targetAxisIndex: 0,
					color: CONFIG.color.conversion
				}
			}
		};

		var chart = new google.visualization.LineChart(document.getElementById('lineView-conversions'));
		chart.draw(data, options);

		isConversionsGraphShown = true;
	}
</script>

<div class="menu menu-section">
	<ul>
		<li><strong>Show :</strong></li>
		<li><a href="" class="button" id="btn_section_0" onclick="javascript:showSection(0, false); return false;">Global</a></li>
		<li><a href="" class="button" id="btn_section_1" onclick="javascript:showSection(1, false); return false;">Visits</a></li>
		<li><a href="" class="button" id="btn_section_2" onclick="javascript:showSection(2, false); return false;">URLs</a></li>
		<li><a href="" class="button" id="btn_section_3" onclick="javascript:showSection(3, false); return false;">Events</a></li>
		<li><a href="" class="button" id="btn_section_4" onclick="javascript:showSection(4, false); return false;">Conversions</a></li>
	</ul>
</div>

<div id="section_0" class="section section-global">

	<div class="subtables-total">
		<div>
			<table>
				<tr>
					<th colspan="4"><strong>Global Visits</strong></th>
				</tr>
				<tr>
					<th>Total Visits</th>
					<th>Avg Visits per Visitors</th>
					<th>Visits for 1 Event</th>
					<th>Visits for 1 Conversion</th>
				</tr>
				<tr>
					<td><?php print $data->total->visits; ?></td>
					<td><?php print ($data->total->visitors == 0)? 0 : round($data->total->visits / $data->total->visitors, 3); ?></td>
					<td><?php print ($data->total->events == 0)? 0 : round($data->total->visits / $data->total->events, 3); ?></td>
					<td><?php print ($data->total->conversions == 0)? 0 : round($data->total->visits / $data->total->conversions, 3); ?></td>
				</tr>
			</table>
		</div>
		<br clear="both"/>

		<div>
			<table>
				<tr>
					<th colspan="2"><strong>Global Events</strong></th>
				</tr>
				<tr>
					<th>Avg pages per Event</th>
					<th>Avg time per Event</th>
				</tr>
				<tr>
					<td><?php print ($data->total->events == 0)? 0 :  round($data->global_events->total_pages / $data->total->events, 3); ?></td>
					<td><?php print $data->avg->time_per_event; ?></td>
				</tr>
			</table>
		</div>
		<br clear="both"/>

		<div>
			<table>
				<tr>
					<th colspan="2"><strong>Global Conversions</strong></th>
				</tr>
				<tr>
					<th>Avg pages per Conversion</th>
					<th>Avg time per Conversion</th>
				</tr>
				<tr>
					<td><?php print ($data->total->conversions == 0)? 0 : round($data->global_conversions->total_pages / $data->total->conversions, 3); ?></td>
					<td><?php print $data->avg->time_per_conversion; ?></td>
				</tr>
			</table>
		</div>
	</div>
	<div class="container-graph container-graph-medium">
		<div class="graph graph-large" style="height:420px; width:100%; display: block; margin:0;" id="comboView-global"></div>
	</div>

</div>

<div id="section_1" style="display:none" class="section section-visits">
	<!-- <h2>Visits</h2> -->
	<div id="container_lineView-visits" class="container-graph container-graph-large">
		<div class="graph graph-large" style="height:200px; width:100%; display: block; margin:0;" id="lineView-visits"></div>
	</div>
	<div class="tables-visits">
		<div class="half half-first">
			<table class="top-visitors">
				<tr>
					<th colspan="5"><strong>Top <?php print $data->top_visitors->limit; ?> Visitors</strong></th>
				</tr>
				<tr>
					<th>User ID</th>
					<th>User Name</th>
					<th>User Info</th>
					<th>Total Visits</th>
				</tr>
				<?php $index = 1; ?>
				<?php if (isset($data->top_visitors->list)) : ?>
					<?php foreach ($data->top_visitors->list as $visitor): ?>
						<tr class="<?php print ($index%2)? 'even' : 'odd'; ?>">
							<?php if($visitor->uid == NULL): ?>
								<td>-</td>
								<td>Anonymous</td>
								<td>-</td>
								<td><?php print $visitor->total_visits; ?></td>
							<?php else: ?>
								<td><?php print $visitor->uid; ?></td>
								<td><?php print $visitor->user->name; ?></td>
								<td><a href="/user/<?php print $visitor->uid; ?>/tracking">User Tracking Tool</a></td>
								<td><?php print $visitor->total_visits; ?></td>							
							<?php endif; ?>				
						</tr>
						<?php $index++; ?>
					<?php endforeach; ?>
				<?php endif; ?>
			</table>
		</div>

		<div class="half">
			<table>
				<tr>
					<th colspan="4"><strong>Most <?php print $data->recent_visits->limit; ?> Recent Visits</strong></th>
				</tr>
				<tr>
					<th>Link Name</th>
					<th>Short Url</th>
					<th>User</th>
					<th>When</th>
				</tr>
				<?php $index = 1; ?>
				<?php if (isset($data->recent_visits->list)) : ?>
					<?php foreach ($data->recent_visits->list as $visit): ?>
						<tr class="<?php print ($index%2)? 'even' : 'odd'; ?>">
							<td><?php print $visit->short_url->analytics_content; ?></td>
							<td><?php print $visit->short_url->admin_link; ?></td>
							<td>
								<?php if($visit->uid == NULL): ?>
									Anonymous
								<?php else: ?>					
									<?php print $visit->user->name; ?><br/><a href="/user/<?php print $visit->uid; ?>/tracking">User Tracking Tool</a>								
								<?php endif; ?>
							</td>
							<td><?php print $visit->ago; ?></td>						
						</tr>
						<?php $index++; ?>
					<?php endforeach; ?>
				<?php endif; ?>
			</table>
		</div>
	</div>
	<br clear="both"/>	
</div>

<div id="section_2" style="display:none" class="section section-urls">
	<!-- <h2>URLS</h2> -->
	<div id="container_lineView-urls" class="container-graph container-graph-large">
		<div class="graph graph-large" style="height:200px; width:100%; display: block; margin:0;" id="lineView-urls"></div>
	</div>
	<div class="tables-urls">
		<div class="half half-first">
			<table>
				<tr>
					<th colspan="5"><strong>Top <?php print $data->top_urls->limit; ?> Urls</strong></th>
				</tr>
				<tr class="cols-5">
					<th>Link Name</th>
					<th>Campaign</th>
					<th>Short Url</th>
					<th>Visits</th>
					<th>Info</th>
				</tr>
				<?php $index = 1; ?>
				<?php if (isset($data->top_urls->list)) : ?>
					<?php foreach ($data->top_urls->list as $url): ?>
						<tr class="<?php print ($index%2)? 'even' : 'odd'; ?>">
							<td><?php print $url->analytics_content; ?></td>
							<td><?php print (isset($data->isCampaign) && $data->isCampaign) ? $url->campaign_name : $url->campaign_admin_link; ?></td>
							<td><?php print $url->admin_link; ?></td>
							<td><?php print $url->visits; ?></td>
							<td><a href="#" onclick="javascript:show_more('top-url', <?php print $url->uid; ?>);return false;">More</a></td>
						</tr>		
						<tr style="display: none;" id="top-url-<?php print $url->uid; ?>" class="<?php print ($index%2)? 'even' : 'odd'; ?> container-more">
							<td colspan="5">
								<table style="border: 0;">
									<tr>
										<td class="inner">
											<strong>Date of creation: </strong><br/>
											<?php print $url->created; ?>
										</td>
										<td class="inner right">
											<strong>Campaign size: </strong><br/>
											<?php print $url->campaign_size; ?>
										</td>
									</tr>
									<tr>
										<td class="inner bottom">
											<strong>Percent visits/campaign size: </strong><br/>
											<?php print $url->visit_percent; ?>
										</td>
										<td class="inner right bottom">
											<strong>Destination: </strong><br/>
											<?php print $url->url_base; ?>
										</td>
									</tr>
								</table>
								<div>
									<a href="#" onclick="javascript:show_more('top-url', <?php print $url->uid; ?>);return false;">Less</a>
								</div>
							</td>			
						</tr>
						<?php $index++; ?>
					<?php endforeach; ?>
				<?php endif; ?>
			</table>
		</div>

		<?php $title_urls = (isset($data->isCampaign) && $data->isCampaign) ? 'Campaign\'s Urls' : '5 latest Urls';?>

		<div class="half">
			<table>
				<tr>
					<th colspan="5"><strong><?php print $title_urls; ?></strong></th>
				</tr>
				<tr>
					<th>Link Name</th>
					<th>Campaign</th>
					<th>Short Url</th>
					<th>Visits</th>
					<th>Info</th>
				</tr>
				<?php $index = 1; ?>
				<?php if (isset($data->all_urls->list)) : ?>
					<?php foreach ($data->all_urls->list as $url): ?>
						<tr class="<?php print ($index%2)? 'even' : 'odd'; ?>">
							<td><?php print $url->analytics_content; ?></td>
							<td><?php print (isset($data->isCampaign) && $data->isCampaign) ? $url->campaign_name : $url->campaign_admin_link; ?></td>
							<td><?php print $url->admin_link; ?></td>
							<td><?php print $url->visits; ?></td>
							<td><a href="#" onclick="javascript:show_more('recent-url', <?php print $url->uid; ?>); return false;">More</a></td>
						</tr>		
						<tr style="display: none;" id="recent-url-<?php print $url->uid; ?>" class="<?php print ($index%2)? 'even' : 'odd'; ?> container-more">
							<td colspan="5">
								<table style="border: 0;">
									<tr>
										<td class="inner">
											<strong>Date of creation: </strong><br/>
											<?php print $url->created; ?>
										</td>
										<td class="inner right">
											<strong>Campaign size: </strong><br/>
											<?php print $url->campaign_size; ?>
										</td>
									</tr>
									<tr>
										<td class="inner bottom">
											<strong>Percent visits/campaign size: </strong><br/>
											<?php print $url->visit_percent; ?>
										</td>
										<td class="inner right bottom">
											<strong>Destination: </strong><br/>
											<?php print $url->url_base; ?>
										</td>
									</tr>
								</table>
								<div>
									<a href="#" onclick="javascript:show_more('recent-url', <?php print $url->uid; ?>); return false;">Less</a>
								</div>
							</td>			
						</tr>
						<?php $index++; ?>
					<?php endforeach; ?>
				<?php endif; ?>
			</table>
		</div>
	</div>
	<br clear="both"/>
</div>

<div id="section_3" style="display:none">
	<!-- <h2>Events</h2> -->
	<div id="container_lineView-events" class="container-graph container-graph-large">
		<div class="graph graph-large" style="height:200px; width:100%; display: block; margin:0;" id="lineView-events"></div>
	</div>
	<div class="fourth fourth-first">
		<table>
			<tr>
				<th colspan="2"><strong>Top <?php print $data->top_url_by_events->limit; ?> Urls by Events</strong></th>
			</tr>
			<tr>
				<th>Short Url</th>
				<th>Events</th>
			</tr>
			<?php $index = 1; ?>
			<?php if (isset($data->top_url_by_events->list)) : ?>
				<?php foreach ($data->top_url_by_events->list as $url): ?>
					<tr class="<?php print ($index%2)? 'even' : 'odd'; ?>">
						<td><?php print $url->admin_link; ?></td>
						<td><?php print $url->total_events; ?></td>						
					</tr>
					<?php $index++; ?>
				<?php endforeach; ?>
			<?php endif; ?>
		</table>
	</div>

	<div class="fourth fourth-last container-recent-events">
		<table class="recent-events">
			<tr>
				<th colspan="6"><strong>Most <?php print $data->recent_events->limit; ?> Recent Events</strong></th>
			</tr>
			<tr>
				<th>Who</th>
				<th>What</th>
				<th>When</th>
				<th>Short Link</th>
				<th>5 last Visits before Event</th>
				<th>Duration</th>
			</tr>
			<?php $index = 1; ?>
			<?php if (isset($data->recent_events->list)) : ?>
				<?php foreach ($data->recent_events->list as $key => $event): ?>
					<?php $tracking = $event->tracking; ?>
					<tr class="<?php print ($index%2)? 'even' : 'odd'; ?>">
						<td>
							<?php if($event->uid == NULL): ?>
								Anonymous
							<?php else: ?>					
								<?php print $event->user->name; ?><br/><a href="/user/<?php print $event->uid; ?>/tracking">User Tracking Tool</a>								
							<?php endif; ?>	
						</td>
						<td>
							<?php print $tracking->event->url; ?>
						</td>
						<td>
							<?php print $event->ago; ?>
						</td>
						<td>
							<?php print $tracking->entry->extra->admin_link; ?>
						</td>
						<td style="width: 40%;">
							<fieldset class="collapsible form-wrapper collapsed">
								<legend>
									<span class="fieldset-legend">Urls</span>
								</legend>
								<div class="fieldset-wrapper">							
									<ul>
										<li><strong>Entry</strong>: <?php print $tracking->entry->url; ?></li>
										<?php if($tracking->size > 5): ?>
											<li>... <?php print $tracking->size - 5; ?> More urls</li>	
										<?php endif; ?>
										<?php foreach ($tracking->urls as $key => $visit): ?>
											<li><?php print $visit->url; ?></li>								
										<?php endforeach; ?>
										<li><strong>Event</strong>: <?php print $tracking->event->url; ?></li>
									</ul>	
								</div>						
							</fieldset>
						</td>						
						<td>
							<?php print $event->duration; ?>
						</td>
					</tr>
					<?php $index++; ?>
				<?php endforeach; ?>
			<?php endif; ?>
		</table>
	</div>
</div>

<div id="section_4" style="display:none">
	<!-- <h2>Conversions</h2> -->
	<div id="container_lineView-conversions" class="container-graph container-graph-large">
		<div class="graph graph-large" style="height:200px; width:100%; display: block; margin:0;" id="lineView-conversions"></div>
	</div>
	<div class="fourth fourth-first">
		<table>
			<tr>
				<th colspan="2"><strong>Top <?php print $data->top_url_by_conversions->limit; ?> Urls by Conversion</strong></th>
			</tr>
			<tr>
				<th>Short Url</th>
				<th>Conversions</th>
			</tr>
			<?php $index = 1; ?>
			<?php if (isset($data->top_url_by_conversions->list)) : ?>
				<?php foreach ($data->top_url_by_conversions->list as $url): ?>
					<tr class="<?php print ($index%2)? 'even' : 'odd'; ?>">
						<td><?php print $url->admin_link; ?></td>
						<td><?php print $url->total_conversions; ?></td>						
					</tr>
					<?php $index++; ?>
				<?php endforeach; ?>
			<?php endif; ?>
		</table>
	</div>

	<div class="fourth fourth-last container-recent-conversions">
		<table class="recent-events">
			<tr>
				<th colspan="6"><strong>Most <?php print $data->recent_conversions->limit; ?> Recent Conversions</strong></th>
			</tr>
			<tr>
				<th>Who</th>
				<th>What</th>
				<th>When</th>
				<th>Short Link</th>
				<th>5 last Visits before Conversion</th>
				<th>Duration</th>
			</tr>
			<?php $index = 1; ?>
			<?php if (isset($data->recent_conversions->list)) : ?>
				<?php foreach ($data->recent_conversions->list as $key => $conversion): ?>
					<?php $tracking = $conversion->tracking; ?>
					<tr class="<?php print ($index%2)? 'even' : 'odd'; ?>">
						<td>
							<?php if($conversion->uid == NULL): ?>
								Anonymous
							<?php else: ?>					
								<?php print $conversion->user->name; ?><br/><a href="/user/<?php print $conversion->uid; ?>/tracking">User Tracking Tool</a>								
							<?php endif; ?>	
						</td>
						<td>
							<?php print $tracking->conversion->url; ?>
						</td>
						<td>
							<?php print $conversion->ago; ?>
						</td>
						<td>
							<?php print $tracking->entry->extra->admin_link; ?>
						</td>
						<td style="width: 40%;">
							<fieldset class="collapsible form-wrapper collapsed">
								<legend>
									<span class="fieldset-legend">Urls</span>
								</legend>
								<div class="fieldset-wrapper">							
									<ul>
										<li><strong>Entry</strong>: <?php print $tracking->entry->url; ?></li>
										<?php if($tracking->size > 5): ?>
											<li>... <?php print $tracking->size - 5; ?> More urls</li>	
										<?php endif; ?>
										<?php foreach ($tracking->urls as $key => $visit): ?>
											<li><?php print $visit->url; ?></li>								
										<?php endforeach; ?>
										<li><strong>Conversion</strong>: <?php print $tracking->conversion->url; ?></li>
									</ul>	
								</div>						
							</fieldset>
						</td>						
						<td>
							<?php print $conversion->duration; ?>
						</td>
					</tr>
					<?php $index++; ?>
				<?php endforeach; ?>
			<?php endif; ?>
		</table>
	</div>
</div>
