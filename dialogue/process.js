const fs = require('fs');

const args = process.argv.slice(2);

if (args.length < 1) {
	throw new Error("No arguments passed in! No dialogue file to parse!");
}


const fileData = args.map((arg) => { return fs.readFileSync(arg, { encoding: 'utf8' }); });

const breakUpConvos = (stringData) => {
	return stringData.split('\n\n');
};
const rawConvos = fileData.map((data) => { return breakUpConvos(data); }).flat();

const convosByLine = rawConvos.map((data) => { return data.split('\n'); });


// Adapted from:
// https://stackoverflow.com/questions/42775222/filter-all-hashtags-found-in-string
const extractSpeakers = (line) => {
	const search = line.match(/@\w+/g);
	if (search === null) {
		return [];
	}

	return search.map(v => v.replace('@', ''));
}

const extractFlags = (line) => {
	const search = line.match(/#\w+/g);
	if (search === null) {
		return [];
	}

	return search.map(v => v.replace('#', ''));
}

// copied from:
// https://stackoverflow.com/questions/3514357/strip-hashtags-from-string-using-javascript
const removeSpeakersAndFlags = (rawLine) => {
	var flagRegex = new RegExp('#([^\\s]*)','g');
	var speakerRegex = new RegExp('@([^\\s]*)','g');

	return rawLine.replace(flagRegex, '').replace(speakerRegex, '');
}

const lineConvosToJSON = (convoLines) => {
	// filter out empty convos
	if (convoLines.length < 1) {
		return null;
	}

	if (convoLines.length < 2) {
		// filter out empty strings
		if (convoLines[0].length < 1) {
			return null;
		}

		console.error('WARNING! ' + convoLines[0] + ' being processed weird; skiping')
	}

	// Get rid of empty lines
	convoLines = convoLines.filter(e => e !== '');

	const result = {
		key: convoLines[0],
		lines: []
	};

	convoLines.forEach((line, i) => {
		// Don't include the convo key
		if (i === 0) {
			return;
		}
		const speakers = extractSpeakers(line);
		const flags = extractFlags(line);

		// TODO: remove any initial space
		const pureLine = removeSpeakersAndFlags(line);

		const lineData = {
			text: pureLine,
			speaker: speakers.length > 0 ? speakers[0] : '',
			flags: flags
		}

		result.lines.push(lineData);
	});

	return result;
};
const processedConvos = convosByLine.map(lineConvosToJSON);
const processedConvosWithoutEmpties = processedConvos.filter(convo => convo !== null);

processedConvosWithoutEmpties.forEach((convoItem) => {
	fs.writeFile('processed/' + convoItem.key + '.json', JSON.stringify(convoItem.lines, null, 4), (err) => {
		if (err) {
			throw err;
		}

		console.log('successfully written ' + convoItem.key);
	});
})