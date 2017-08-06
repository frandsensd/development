
// Components.utils.import("resource://gre/modules/Timer.jsm");
// let intervalID = setInterval(function() { console.log("Happening every 500ms!"); }, 500);
// clearInterval(intervalID);
var slideNumber;
var numberOfSlides;
var timerId;

function showSlide() {
	slideNumber++;
	console.log('Showing slide ', slideNumber);
	if (slideNumber === numberOfSlides) {
		stopSlideShow();
	}
}

function stopSlideShow() {
	if (timerId) {
		clearInterval(timerId);
		console.log('Slideshow timer stopped');
	}
	console.log('Slideshow stopped');
	let slidePage = document.getElementById('slide');
	if (slidePage.style.visibility === 'visible') {
		slidePage.style.visibility = 'hidden';
	}

	let frontPage = document.getElementById('front');
	if (frontPage.style.visibility === 'hidden') {
		frontPage.style.visibility = 'visible';
	}
}

function startKaraoke(event) {
	let sender = event.target;
	let advanceType = sender.form.elements.advanceType.value;
	let autoAdvanceSeconds = parseInt(sender.form.elements.autoAdvanceSeconds.value);
	numberOfSlides = parseInt(sender.form.elements.numberOfSlides.value);
	slideNumber = 0;
	isRunning = false;
	if (timerId) {
		clearInterval(timerId);
		console.log('Previous slideshow stopped');
	}
	console.log('startKaraoke() triggered by sender:' + sender.className);
	console.log('advanceType: ' + advanceType);
	console.log('autoAdvanceSeconds: ' + autoAdvanceSeconds);
	console.log('numberOfSlides: ' + numberOfSlides);

	let slidePage = document.getElementById('slide');
	if (slidePage.style.visibility === 'hidden') {
		slidePage.style.visibility = 'visible';
	}

	let frontPage = document.getElementById('front');
	if (frontPage.style.visibility === 'visible') {
		frontPage.style.visibility = 'hidden';
	}

	if (advanceType === 'auto') {
		timerId = setInterval(showSlide, (autoAdvanceSeconds*1000));
		console.log('Slideshow timer started');
	}
	else {
		advanceSlideManually();
	}
	return false;
};

function advanceSlideManually(event) {
	console.log('manual slide advance');
	showSlide();
}
