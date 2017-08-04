const fs = require('fs');

var walkSync = function(dir, filelist) {
	var fs = fs || require('fs'),
		files = fs.readdirSync(dir);
		filelist = filelist || [];
	files.forEach(function(file) {
			if (fs.statSync(dir + file).isDirectory()) {
			filelist = walkSync(dir + file + '/', filelist);
		} else {
			if (file.indexOf('.h') > 0 || file.indexOf('.c') > 0)
				filelist.push(dir + file);
		}
	});
	return filelist;
};

console.log(walkSync('src/'));

function processFile(file) {
	var data = fs.readFileSync(file).toString().split('\n');
	console.log('Processing', file);
	data.unshift('');
	data.unshift('#include "' + '../'.repeat(file.split('/').length - 2) + "xorstring.hpp\"");
	data.unshift('');
	for (var i = 0; i < data.length; i++) {
		if (data[i].indexOf('#include') < 0 && data[i].indexOf('extern \"C\"') < 0) {
			var d = data[i];
			d = d.replace(/\\\\/g, "{{{PP:DOUBLEESCAPE}}}");
			d = d.replace(/\\"/g, "{{{PP:QUOTATION}}}");
			d = d.replace(/"(.*?)"/g, "XStr(\"$1\")");
			d = d.replace(/\{\{\{PP:DOUBLEESCAPE\}\}\}/g, "\\\\");
			d = d.replace(/\{\{\{PP:QUOTATION\}\}\}/g, "\\\"");
			data[i] = d;
		}
	}
	fs.writeFileSync(file, data.join('\n'));
}
var fl = walkSync('src/');

for (var i = 0; i < fl.length; i++) {
	processFile(fl[i]);
}