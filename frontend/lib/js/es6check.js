/*! es6check.js - Eugene Yemelyanov - MIT Licensed */
(function () {
	"use strict";
	var supportsES6 = function() {
		try {
			new Function("() => {}");
			return true;
		}
		catch (err) {
			return false;
		}
	}();
	if ( !supportsES6 ) {
		var lang = window.navigator.browserLanguage;
		switch ( lang ) {
			case 'ru-RU':
				var title = "Ваш веб-браузер не поддерживается";
				var message = "Ваш веб-браузер устарел и больше не поддерживается.<br/>Пожалуйства обновите ваш веб-браузер или используйте другой.";
			default:
				var title = "Your web browser is not supported";
				var message = "Your web browser is obsolete and no longer is supported.<br/>Please upgrade your web brower or use an alternative one.";
		}
		document.write("<h1>"+title+"</h1>");
		document.write("<p>"+message+"</p>");
		document.execCommand("Stop");
	}
})();

