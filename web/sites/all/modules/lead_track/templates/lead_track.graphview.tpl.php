<?php

?>


<script
type="text/javascript" src="https://www.google.com/jsapi"></script>
<script type="text/javascript">

	function toggle() {

		var el = document.getElementById("graphView");

		el.style.display = (el.style.display != 'none' ? 'none' : 'block' );

		var elPie = document.getElementById("pieView");

		if(elPie.style.display != 'none'){

			elPie.style.display = 'none';
			document.getElementById("btn_switchGraph").innerHTML = 'Show Pie View';

		}else{

			elPie.style.display = 'block';
			drawPieChart();
			document.getElementById("btn_switchGraph").innerHTML = 'Show Column View';	
		}


	}

	// Load the Visualization API and the piechart package.
	google.load('visualization', '1.0', {'packages':['corechart']});

	// Set a callback to run when the Google Visualization API is loaded.
	google.setOnLoadCallback(drawGraphs);


	function drawGraphs(){
		//Google is loaded so we can draw all the graphs
		preparePieData();
		drawChart();		
	}

	// Callback that creates and populates a data table,
	// instantiates the pie chart, passes in the data and
	// draws it.
	function drawChart() {

		
		
		<?php $graphData = $data->graph; ?>

	// Create the data table.
	var data = new google.visualization.DataTable({
		cols: <?php echo $graphData->cols; ?>,
		rows: <?php echo $graphData->rows; ?>
	});
	

	// Set chart options
	var options = {'title':'Visits and Size per Campaign',
	height: 480,
	chartArea: {left:60, right: 0, width:'87%'},
	legend: {
		position: 'top'
	},
	vAxes: {
		0: {
			logScale: false,
			minValue: 0,
			format:'#',
			textStyle: {color: CONFIG.color.visit}

		},
		1: {
			logScale: false,
			minValue: 0,
			format:'#',
			textStyle: {color: 'red'}
		}
	}
	<?php 
	echo @$graphData->options;			
	?>

};

<?php if($graphData->isSelectedCampaign): ?>
		// Instantiate and draw our chart, passing in some options.
		var chart = new google.visualization.ComboChart(document.getElementById('graphView'));

		<?php else:	?>
		// Instantiate and draw our chart, passing in some options.
		var chart = new google.visualization.ColumnChart(document.getElementById('graphView'));

		<?php endif; ?>

		chart.draw(data, options);

	}

	var PIE_DATA;
	var PIE_OPTIONS;
	var PIE_CHART;

	function preparePieData(){
		<?php $pieData = $data->pie; ?>

		// Create the data table.
		PIE_DATA = new google.visualization.DataTable({
			cols: <?php echo $pieData->cols; ?>,
			rows: <?php echo $pieData->rows; ?>
		});

		PIE_OPTIONS = {
			title: 'Visit shares',
			height: 480,
			chartArea: {left:60, right: 0, width:'90%'},
			legend: {
				position: 'right'
			}

		};

		PIE_CHART = new google.visualization.PieChart(document.getElementById('pieView'));
	}

	// Callback that creates and populates a data table,
	// instantiates the pie chart, passes in the data and
	// draws it.
	function drawPieChart() {
		PIE_CHART.draw(PIE_DATA, PIE_OPTIONS);                             
	}

	
	

</script>
<div id="container_graphview">
	<h2>The GraphView</h2>
	<div id="graphView"></div>
	<div id="pieView" style="display: none;"></div>
	<button id="btn_switchGraph" onclick="javascript:toggle();return false;">Show Pie View</button>
</div>
