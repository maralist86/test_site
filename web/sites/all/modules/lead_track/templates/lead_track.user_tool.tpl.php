<?php
$debug = FALSE;//$data->user;
if($debug){
	print '<pre>';
	print_r($data);
	print '</pre>';
}
?>

<script
type="text/javascript" src="https://www.google.com/jsapi"></script>
<script type="text/javascript">

	/* TODO: Add Google Charts Api code to generate the charts */

	function toggle(obj) {

		var el = document.getElementById(obj);

		el.style.display = (el.style.display != 'none' ? 'none' : '' );

	}

</script>
<div id="container_user_tool">
	<div id="id-card">
		<?php $user = &$data->user; ?>
		<h2>User Info</h2>
		<div class="container container-personal">
			<h3>
				<?php print $user->name; ?>
			</h3>
			<p>
				<strong>Status :</strong>
				<?php print ($user->status) ? 'Active' : 'Blocked'; ?>
			</p>


			<p>
				<strong>Email :</strong> <a href="mailto:<?php print $user->mail; ?>"><?php print $user->mail; ?>
			</a>
		</p>


		<p>
			<strong>Time zone :</strong>
			<?php print $user->timezone; ?>
		</p>

	</div>
	<div class="container container-logs">
		<h3>Logs</h3>
		<p>
			<strong>Account created :</strong>
			<?php print $user->created_ago . '&nbsp;(' . date('m/d/Y H:i:s', $user->created) . ')'; ?>
		</p>


		<p>
			<strong>Last login :</strong>
			<?php print $user->login_ago . '&nbsp;(' . date('m/d/Y H:i:s', $user->login) . ')'; ?>
		</p>


		<p>
			<strong>Last access :</strong>
			<?php print $user->access_ago . '&nbsp;(' . date('m/d/Y H:i:s', $user->access) . ')'; ?>
		</p>

	</div>
	<div class="container container-other">
		<fieldset class="collapsible form-wrapper collapsed">
			<legend>
				<span class="fieldset-legend">Extra Data</span>
			</legend>
			<div class="fieldset-wrapper">
				<table>						
					<?php foreach($user->extra as $key => $value): ?>
						<?php if(!empty($value)) : ?>
							<tr>
								<th>
									<strong><?php print $key; ?> :</strong>
								</th>
								<td>
									<?php if(is_array($value) && isset($value['#markup'])) : ?>
										<?php print $value['#markup']; ?>
									<?php elseif (!is_array($value)) : ?>
										<?php print htmlentities($value); ?>
									<?php endif; ?>
								</td>
							</tr>
						<?php endif; ?>						
					<?php endforeach; ?>
				</table>
			</div>						
		</fieldset>			
	</div>
</div>
<br clear="both" />
<div id="drupal-data">

	<h2>Data from Lead Track</h2>

	<div class="container-section">
		<div>
			<div class="section-results" style="width: 100%; margin: 0;">
				<table>
					<tr>
						<th class="bigtext color-score" style="text-align: center"><?php print (isset($user->score)) ? $user->score : 0; ?></th>
						<th class="bigtext color-visit" style="text-align: center"><?php print (isset($user->_global_visits->total_visits)) ? $user->_global_visits->total_visits : 0; ?></th>
						<th class="bigtext color-event" style="text-align: center"><?php print (isset($user->_global_events->total_events)) ? $user->_global_events->total_events : 0; ?></th>								
						<th class="bigtext color-conversion" style="text-align: center"><?php print (isset($user->_global_conversions->total_conversions)) ? $user->_global_conversions->total_conversions : 0; ?></th>
					</tr>				
					<tr>
						<td>Score</td>
						<td>Visits</td>
						<td>Events</td>
						<td>Conversions</td>
					</tr>
				</table>
			</div>
		</div>
	</div>

	<table>
		<tr>
			<th colspan="4"><strong>Global Conversions</strong></th>
		</tr>
		<tr>
			<th>Total conversions</th>
			<th>Avg pages per Conversion</th>
		</tr>
		<tr>
			<td><?php print $user->_global_conversions->total_conversions; ?></td>
			<td><?php print ($user->_global_conversions->total_conversions == 0)? 0 : round($data->global_conversions->total_pages / $user->_global_conversions->total_conversions, 3); ?></td>
		</tr>
	</table>
	<table>
		<tr>
			<th colspan="4"><strong>Global Events</strong></th>
		</tr>
		<tr>
			<th>Total events</th>
			<th>Avg Events per Conversion</th>
		</tr>
		<tr>
			<td><?php print $user->_global_events->total_events; ?></td>
			<td><?php print ($user->_global_conversions->total_conversions == 0)? 0 :  round($user->_global_events->total_events / $user->_global_conversions->total_conversions, 3); ?></td>
		</tr>
	</table>
	<table>
		<tr>
			<th colspan="4"><strong>Global Visits</strong></th>
		</tr>
		<tr>
			<th>Total visits</th>
		</tr>
		<tr>
			<td><?php print $user->_global_visits->total_visits; ?></td>
		</tr>
	</table>

	<script type="text/javascript">

		<?php $js_data = $user->total->js_data; ?>

		function drawChartGlobal() {

			var data = google.visualization.arrayToDataTable([
				['Month', 'Conversions', 'Events', 'Visits'],
				['<?php print $js_data->period_2_ago->name; ?>', <?php print $js_data->period_2_ago->conversions; ?>, <?php print $js_data->period_2_ago->events; ?>, <?php print $js_data->period_2_ago->visits; ?>],
				['<?php print $js_data->period_1_ago->name; ?>', <?php print $js_data->period_1_ago->conversions; ?>, <?php print $js_data->period_1_ago->events; ?>, <?php print $js_data->period_1_ago->visits; ?>],
				['<?php print $js_data->period_current->name; ?>', <?php print $js_data->period_current->conversions; ?>, <?php print $js_data->period_current->events; ?>, <?php print $js_data->period_current->visits; ?>]
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
				},
				isStacked: true

			};

			var chart = new google.visualization.ColumnChart(document.getElementById('comboView-global'));
			chart.draw(data, options);                             
		}

	</script>
	<div class="section section-global">

		<div class="container-graph container-graph-medium">
			<div class="graph graph-large" style="height:420px; width:100%; display: block; margin:0;" id="comboView-global"></div>
		</div>

	</div>

	<div class="container container-drupal">		

		<div class="container container-other">
			<fieldset class="collapsible form-wrapper collapsed">
				<legend>
					<span class="fieldset-legend">Detail Conversions</span>
				</legend>
				<div class="fieldset-wrapper">
					<table class="recent-conversions">
						<tr>
							<th>Campaign</th>
							<th>Link</th>
							<th>What</th>
							<th>When</th>
							<th>5 last Visits before Conversion</th>
							<th>Duration</th>
						</tr>
						<?php $index = 1; ?>
						<?php foreach ($user->conversions as $key => $conversion): ?>
							<?php $tracking = $conversion->tracking; ?>
							<tr class="<?php print ($index%2)? 'even' : 'odd'; ?>">
								<td>
									<?php print $tracking->entry->extra->admin_link_campaign; ?>
								</td>
								<td>
									<?php print $tracking->entry->extra->admin_link; ?>
									<br/>
									<?php print $tracking->entry->extra->analytics_content; ?>
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
					</table>
				</div>
			</fieldset>
		</div>		

		<div class="container container-other">
			<fieldset class="collapsible form-wrapper collapsed">
				<legend>
					<span class="fieldset-legend">Detail Events</span>
				</legend>
				<div class="fieldset-wrapper">
					<table class="recent-events">
						<tr>
							<th>Campaign</th>
							<th>Link</th>
							<th>What</th>
							<th>When</th>
							<th>5 last Visits before Event</th>
							<th>Duration</th>
						</tr>
						<?php $index = 1; ?>
						<?php foreach ($user->events as $key => $event): ?>
							<?php $tracking = $event->tracking; ?>
							<tr class="<?php print ($index%2)? 'even' : 'odd'; ?>">
								<td>
									<?php print $tracking->entry->extra->admin_link_campaign; ?>
								</td>
								<td>
									<?php print $tracking->entry->extra->admin_link; ?>
									<br/>
									<?php print $tracking->entry->extra->analytics_content; ?>
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
					</table>
				</div>
			</fieldset>
		</div>		

		<div class="container container-other">
			<fieldset class="collapsible form-wrapper collapsed">
				<legend>
					<span class="fieldset-legend">Detail Visits</span>
				</legend>
				<div class="fieldset-wrapper">
					<table class="visits">
						<tr>
							<th>Campaign</th>
							<th>Link Name</th>
							<th>Short Url</th>
							<th>When</th>
						</tr>
						<?php $index = 1; ?>
						<?php foreach ($user->visits as $visit): ?>
							<tr class="<?php print ($index%2)? 'even' : 'odd'; ?>">
								<td><?php print $visit->campaign->admin_link; ?></td>
								<td><?php print $visit->short_url->analytics_content; ?></td>
								<td><?php print $visit->short_url->admin_link; ?></td>
								<td><?php print $visit->ago; ?></td>						
							</tr>
							<?php $index++; ?>
						<?php endforeach; ?>
					</table>
				</div>
			</fieldset>
		</div>	
	</div>
</div>
<br clear="both" />
</div>
<script type="text/javascript">

	(function($) {		

		google.load('visualization', '1.0', {'packages':['corechart']});
		google.setOnLoadCallback(drawChartGlobal);

	})(jQuery)

</script>