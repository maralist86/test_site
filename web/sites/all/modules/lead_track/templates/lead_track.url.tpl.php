<?php
$debug = FALSE;//$data->user;
if($debug){
	print '<pre>';
	print_r($data);
	print '</pre>';
}
?>


<div class="container-section" id="container_url">

	<div class="section section-rank">
		<div>
			<div>
				<table>
					<tr>
						<th class="bigtext color-score"><?php print (isset($data->total)) ? $data->total->score : 0; ?></th>
						<th class="bigtext color-ranking"><?php print (isset($data->total)) ? $data->total->ranking : 0; ?></th>
					</tr>				
					<tr>
						<td>Score</td>
						<td>Rank in Campaign</td>
					</tr>
				</table>
			</div>
		</div>
	</div>


	<div class="section section-results">
		<div>
			<div>
				<table>
					<tr>
						<th>Visits</th>
						<td class="mediumtext color-visit"><?php print (isset($data->total)) ? $data->total->visits : 0; ?></td>
					</tr>
					<tr>
						<th>Events</th>
						<td class="mediumtext color-event"><?php print (isset($data->total)) ? $data->total->events : 0; ?></td>		
					</tr>
					<tr>
						<th>Conversions</th>
						<td class="mediumtext color-conversion"><?php print (isset($data->total)) ? $data->total->conversions : 0; ?></td>
					</tr>
				</table>
			</div>
		</div>
	</div>


	<div class="section section-card">
		<div>
			<div>
				<table>					
					<tr>
						<th>Base Url</th>
						<td><?php print $data->card->url_base; ?></td>
					</tr>	
					<tr>
						<th>Content</th>
						<td><?php print $data->card->analytics_content; ?></td>
					</tr>				
					<tr>
						<th>Link</th>
						<td><?php print $data->card->copyToClipBoard; ?></td>
					</tr>		
					<tr>
						<th>Source/Medium</th>
						<td><?php print $data->card->analytics_source; ?>/<?php print $data->card->analytics_medium; ?></td>
					</tr>				
					<tr>
						<th>Created</th>
						<td><?php print $data->card->created; ?></td>
					</tr>				
					<tr>
						<th>Campaign</th>
						<td><?php print $data->card->campaign_link; ?></td>
					</tr>
				</table>
			</div>
		</div>
	</div>

	<br clear="both" />
	<hr/>

	<?php $js_data = $data->total->js_data; ?>

	<script type="text/javascript" src="https://www.google.com/jsapi"></script>

	<script type="text/javascript">

		var isUrlsGraphShown = false;
		var isVisitsGraphShown = false;
		var isEventsGraphShown = false;
		var isConversionsGraphShown = false;
		var shownSectionId = 0;

		function showSection(id, isFirstTime) {

			if(id != shownSectionId || isFirstTime){

				if(!isFirstTime){
					//Hide others
					var elToHide = document.getElementById("section_" + shownSectionId);
					elToHide.style.display = 'none';
					document.getElementById("btn_section_" + shownSectionId).className = 'button';
				}
				//Show section
				var elToShow = document.getElementById("section_" + id);
				elToShow.style.display = 'block';
				document.getElementById("btn_section_" + id).className = 'button active';

				if(id == 0 && !isVisitsGraphShown){
					drawChartVisits();
				}else if (id == 1 && !isUrlsGraphShown) {
					drawChartUrls();
				}else if (id == 2 && !isEventsGraphShown) {
					drawChartEvents();
				}else if (id == 3 && !isConversionsGraphShown) {
					drawChartConversions();
				};


				//Save shown section's ID
				shownSectionId = id;
			}
		};

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

		function showDefaultGraphs(){
			showSection(shownSectionId, true);
		}

	</script>

	<div class="menu menu-section">
		<ul>
			<li><strong>Show :</strong></li>
			<li><a href="" class="button" id="btn_section_0" onclick="javascript:showSection(0, false); return false;">Visits</a></li>				
			<li><a href="" class="button" id="btn_section_2" onclick="javascript:showSection(2, false); return false;">Events</a></li>
			<li><a href="" class="button" id="btn_section_3" onclick="javascript:showSection(3, false); return false;">Conversions</a></li>
		</ul>
	</div>

	<div id="section_0" style="display:none" class="section section-visits">
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
						<th>User ID</th>
						<th>User Name</th>
						<th>User Info</th>
						<th>When</th>
					</tr>
					<?php $index = 1; ?>
					<?php if (isset($data->recent_visits->list)) : ?>
						<?php foreach ($data->recent_visits->list as $visit): ?>
							<tr class="<?php print ($index%2)? 'even' : 'odd'; ?>">
								<?php if($visitor->uid == NULL): ?>
									<td>-</td>
									<td>Anonymous</td>
									<td>-</td>
								<?php else: ?>
									<td><?php print $visitor->uid; ?></td>
									<td><?php print $visitor->user->name; ?></td>
									<td><a href="/user/<?php print $visitor->uid; ?>/tracking">User Tracking Tool</a></td>
								<?php endif; ?>	
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

	<div id="section_2" style="display:none">
		<!-- <h2>Events</h2> -->
		<div id="container_lineView-events" class="container-graph container-graph-large">
			<div class="graph graph-large" style="height:200px; width:100%; display: block; margin:0;" id="lineView-events"></div>
		</div>

		<div class="container-recent-events">
			<table class="recent-events">
				<tr>
					<th colspan="6"><strong>Most <?php print $data->recent_events->limit; ?> Recent Events</strong></th>
				</tr>
				<tr>
					<th>Who</th>
					<th>What</th>
					<th>When</th>
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

	<div id="section_3" style="display:none">
		<!-- <h2>Conversions</h2> -->
		<div id="container_lineView-conversions" class="container-graph container-graph-large">
			<div class="graph graph-large" style="height:200px; width:100%; display: block; margin:0;" id="lineView-conversions"></div>
		</div>
		<div class="container-recent-conversions">
			<table class="recent-events">
				<tr>
					<th colspan="6"><strong>Most <?php print $data->recent_conversions->limit; ?> Recent Conversions</strong></th>
				</tr>
				<tr>
					<th>Who</th>
					<th>What</th>
					<th>When</th>
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


</div>

<script type="text/javascript">

	(function($) {			

		google.load('visualization', '1.0', {'packages':['corechart']});
		google.setOnLoadCallback(showDefaultGraphs);

	})(jQuery)

</script>