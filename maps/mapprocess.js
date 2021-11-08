const fs = require('fs');
const buffer = require('buffer').Buffer;

const sourceDir = 'source';

const names = fs.readdirSync(sourceDir);

const jsonDataBuffers = names.map(((path, i) => {
  const jsonData = JSON.parse(fs.readFileSync(sourceDir + '/' + path, { encoding: 'utf-8' }))
  jsonData['name'] = names[i].split('.')[0];
  return jsonData;
}));

const MAX_NUMBER_OF_INGAME_PIECES = 24;
const MAX_NUMBER_OF_INGAME_MONSTERS = 8;
const MAX_NUMBER_OF_PUZZLE_SPACES = 8;


const MONSTER_EDITOR_NAME_OGRE = 'ogre';
const MONSTER_TYPE_OGRE = 0;

const MONSTER_EDITOR_NAME_TOAD = 'toad';
const MONSTER_TYPE_TOAD = 1;

const MONSTER_EDITOR_NAME_SNAKE = 'snake';
const MONSTER_TYPE_SNAKE = 2;

const MONSTER_EDITOR_NAME_JUMPER = 'jumper';
const MONSTER_TYPE_JUMPER = 3;

const MONSTER_EDITOR_NAME_SHADOWQUEEN = 'shadowqueen';
const MONSTER_TYPE_SHADOWQUEEN = 4;

const BOARD_WIDTH = 8;
const BOARD_HEIGHT = 8;

const TILE_INDEX_PUZZLE_SPACE = 13;

const EDITOR_TILE_SIZE = 16;

const processMap = (mapJSON, i) => {
  const result = {
    piecesActive: new Array(MAX_NUMBER_OF_INGAME_PIECES).fill(0),
    pieceX: new Array(MAX_NUMBER_OF_INGAME_PIECES).fill(0),
    pieceY: new Array(MAX_NUMBER_OF_INGAME_PIECES).fill(0),
    pieceType: new Array(MAX_NUMBER_OF_INGAME_PIECES).fill(0),

    activeMonsters: new Array(MAX_NUMBER_OF_INGAME_MONSTERS).fill(0),
    monsterType: new Array(MAX_NUMBER_OF_INGAME_MONSTERS).fill(0),
    monsterX: new Array(MAX_NUMBER_OF_INGAME_MONSTERS).fill(0),
    monsterY: new Array(MAX_NUMBER_OF_INGAME_MONSTERS).fill(0),

    numberOfPuzzleSpots: 0,
    puzzleSpotX: new Array(MAX_NUMBER_OF_PUZZLE_SPACES).fill(0),
    puzzleSpotY: new Array(MAX_NUMBER_OF_PUZZLE_SPACES).fill(0),

    playerX: 0,
    playerY: 0,
    playerRotation: 0,

    startLevelDialogue: '',

    flagA: 0,
    flagB: 0,
    flagC: 0,
    bgm: 0,

    name: names[i].split('.')[0]
  };

  let pieceCount = 0;

  let monsterCount = 0;

  mapJSON.layers.forEach((layer) => {
    if (layer.name === 'board') {
      // console.log(layer.data);
      for (let i = 0; i < (BOARD_WIDTH * BOARD_HEIGHT); i++) {
        if (layer.data[i] === 0) {
          continue;
        }

        const tileIndex = layer.data[i] - 1;
        if (tileIndex === TILE_INDEX_PUZZLE_SPACE) {
          result.puzzleSpotX[result.numberOfPuzzleSpots] = (i % BOARD_WIDTH);
          result.puzzleSpotY[result.numberOfPuzzleSpots] = BOARD_HEIGHT - 1 - ~~(i / BOARD_WIDTH);
          result.numberOfPuzzleSpots++;
        }

        // Chess pieces and walls
        if ((tileIndex <= 5) || (tileIndex === 14)) {
          result.piecesActive[pieceCount] = 1;
          result.pieceX[pieceCount] = (i % BOARD_WIDTH);
          result.pieceY[pieceCount] = BOARD_HEIGHT - 1 - ~~(i / BOARD_WIDTH);
          result.pieceType[pieceCount] = tileIndex;
          pieceCount++;
        }
      }


    } else if (layer.name === 'objects') {
      layer.objects.forEach((object) => {
        // TODO: other monster types
        if (object.type === MONSTER_EDITOR_NAME_OGRE || object.name === MONSTER_EDITOR_NAME_OGRE) {
          result.activeMonsters[monsterCount] = 1;
          result.monsterType[monsterCount] = MONSTER_TYPE_OGRE;
          result.monsterX[monsterCount] = ~~(object.x / EDITOR_TILE_SIZE);
          result.monsterY[monsterCount] = BOARD_HEIGHT - 1 - ~~(object.y / EDITOR_TILE_SIZE);

          monsterCount++;
        } else if (object.type === MONSTER_EDITOR_NAME_TOAD || object.name === MONSTER_EDITOR_NAME_TOAD) {
          result.activeMonsters[monsterCount] = 1;
          result.monsterType[monsterCount] = MONSTER_TYPE_TOAD;
          result.monsterX[monsterCount] = ~~(object.x / EDITOR_TILE_SIZE);
          result.monsterY[monsterCount] = BOARD_HEIGHT - 1 - ~~(object.y / EDITOR_TILE_SIZE);

          monsterCount++;
        } else if (object.type === MONSTER_EDITOR_NAME_SNAKE || object.name === MONSTER_EDITOR_NAME_SNAKE) {
          result.activeMonsters[monsterCount] = 1;
          result.monsterType[monsterCount] = MONSTER_TYPE_SNAKE;
          result.monsterX[monsterCount] = ~~(object.x / EDITOR_TILE_SIZE);
          result.monsterY[monsterCount] = BOARD_HEIGHT - 1 - ~~(object.y / EDITOR_TILE_SIZE);

          monsterCount++;
        } else if (object.type === MONSTER_EDITOR_NAME_JUMPER || object.name === MONSTER_EDITOR_NAME_JUMPER) {
          result.activeMonsters[monsterCount] = 1;
          result.monsterType[monsterCount] = MONSTER_TYPE_JUMPER;
          result.monsterX[monsterCount] = ~~(object.x / EDITOR_TILE_SIZE);
          result.monsterY[monsterCount] = BOARD_HEIGHT - 1 - ~~(object.y / EDITOR_TILE_SIZE);

          monsterCount++;
        } else if (object.type === MONSTER_EDITOR_NAME_SHADOWQUEEN || object.name === MONSTER_EDITOR_NAME_SHADOWQUEEN) {
          result.activeMonsters[monsterCount] = 1;
          result.monsterType[monsterCount] = MONSTER_TYPE_SHADOWQUEEN;
          result.monsterX[monsterCount] = ~~(object.x / EDITOR_TILE_SIZE);
          result.monsterY[monsterCount] = BOARD_HEIGHT - 1 - ~~(object.y / EDITOR_TILE_SIZE);

          monsterCount++;
        } else if (object.type === 'player') {
          result.playerX = ~~(object.x / EDITOR_TILE_SIZE);
          result.playerY = BOARD_HEIGHT - 1 - ~~(object.y / EDITOR_TILE_SIZE);
          result.playerRotation = ~~(object.rotation / 360 * 256) % 256;
        } 
      });
    } else {
      console.warn("WARNING: weird layer name " + layer.name + " was found.");
    }
  });

  mapJSON.properties.forEach((property) => {
    if (property.name === 'dialogue') {
      result.startLevelDialogue = property.value.toString();
    }
  })


  return result;
}


const processedJSON = jsonDataBuffers.map(processMap);

processedJSON.forEach((processedJSON) => {
  fs.writeFile('processed/' + processedJSON.name + '.json', JSON.stringify(processedJSON), { encoding: 'utf8' },(err) => {
    if (err) {
      console.log(err);
    } else {
      console.log('done processing: '+ processedJSON.name) 
    }
  })
})
