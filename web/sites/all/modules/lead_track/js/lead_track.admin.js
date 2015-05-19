jQuery(document).ready(function($) {
	$(".bigtext").bigText({
		padding: 0,
		maximumFontSize: 100,
	});

	$(".mediumtext").bigText({
		padding: 0,
		maximumFontSize: 25,
	});
});

function copyToClipboard(text) {
	window.prompt('Copy to clipboard: Ctrl+C, Enter', text);
}