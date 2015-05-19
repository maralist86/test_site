<?php
$debug = FALSE;//$data->user;
if($debug){
	print '<pre>';
	print_r($data);
	print '</pre>';
}
?>


<div class="container-section" id="container_campaign">

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
						<td>Global Rank</td>
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
						<th>Urls</th>
						<td class="mediumtext color-url"><?php print (isset($data->total)) ? $data->total->links : 0; ?></td>						
					</tr>
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
						<th>Name</th>
						<td><?php print $data->card->name; ?></td>
					</tr>
					<tr>
						<th>Size</th>
						<td><?php print $data->card->size; ?></td>
					</tr>				
					<tr>
						<th>created</th>
						<td><?php print $data->card->created; ?></td>
					</tr>				
					<tr>
						<th>Analytics Events Enabled</th>
						<td>
							<?php print $data->card->ga_event_enable; ?>
							<?php if(!$data->card->ga_event_disable_raw): ?>
								<span>
									&nbsp;(value = <?php print $data->card->ga_event_value; ?>)
								</span>
							<?php endif; ?>
						</td>
					</tr>
					
					<tr>
						<th>Analytics PageView Enabled</th>
						<td><?php print $data->card->ga_pageview_enable; ?></td>
					</tr>				
					<tr>
						<th>SalesForce Enabled</th>
						<td>
							<?php print $data->card->sf_enable; ?>
							<?php if($data->card->sf_enable_raw): ?>
								<span>
									&nbsp;(Source = <?php print $data->card->sf_lead_source; ?>)
								</span>
							<?php endif; ?>
						</td>
					</tr>
				</table>
			</div>
		</div>
	</div>	

	<br clear="both" />
	<hr/>

	<?php if(!isset($data->total)): ?>

		<p>No data found because no urls have been created for this campaign.</p>

	<?php else: ?>

		<script type="text/javascript" src="https://www.google.com/jsapi"></script>

		<?php $js_data = $data->total->js_data; ?>

		<script type="text/javascript">

			var isGlobalGraphShown = false;
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

					if(id == 0 && !isGlobalGraphShown){
						drawChartGlobal();
					}else if (id == 1 && !isVisitsGraphShown) {
						drawChartVisits();
					}else if (id == 2 && !isUrlsGraphShown) {
						drawChartUrls();
					}else if (id == 3 && !isEventsGraphShown) {
						drawChartEvents();
					}else if (id == 4 && !isConversionsGraphShown) {
						drawChartConversions();
					};


					//Save shown section's ID
					shownSectionId = id;
				}
			};

			function show_more(type, id) {

				var el = document.getElementById(type + "-" + id);

				el.style.display = (el.style.display != 'none' ? 'none' : 'table-row' );
			}

			function showDefaultGraphs(){				
				showSection(shownSectionId, true);
			}

		</script>		
		
		<?php include 'includes/section_details.tpl.php'; ?>

	</div>

	<script type="text/javascript">

		(function($) {		

			google.load('visualization', '1.0', {'packages':['corechart']});
			google.setOnLoadCallback(showDefaultGraphs);

		})(jQuery)

	</script>

<?php endif; ?>