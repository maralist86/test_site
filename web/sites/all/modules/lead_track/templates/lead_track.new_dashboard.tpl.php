<?php
$debug = FALSE;//$data->user;
if($debug){
	print '<pre>';
	print_r($data);
	print '</pre>';
}
?>


<div class="container-section" id="container_dashboard">


<div class="section section-results">
	<div>
		<div>
			<table>
				<tr>
					<th class="bigtext color-score"><?php print $data->total->campaigns; ?></th>
					<th class="bigtext color-url"><?php print $data->total->links; ?></th>
					<th class="bigtext color-visit"><?php print $data->total->visits; ?></th>
					<th class="bigtext color-event"><?php print $data->total->events; ?></th>
					<th class="bigtext color-conversion"><?php print $data->total->conversions; ?></th>
				</tr>				
				<tr>
					<td>Campaign</td>
					<td>Urls</td>
					<td>Visits</td>						
					<td>Events</td>
					<td>Conversions</td>
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