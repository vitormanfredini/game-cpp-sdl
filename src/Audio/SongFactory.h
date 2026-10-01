#pragma once

class SongFactory {

public:

    enum class Song {
        Stage1
    };

    SongFactory(AudioEngine* audioEngine){

        DynamicSong stage1Song;

        stage1Song.addSection(
            audioEngine->loadSound("audio/song2/level0.wav"),0,0,0
        );
        stage1Song.addSection(
            audioEngine->loadSound("audio/song2/level1.wav"),0,0,0
        );

        stage1Song.addSection(
            audioEngine->loadSound("audio/song2/level2.wav"),
            audioEngine->loadSound("audio/song2/level2fill.wav"),
            0,
            0
        );

        stage1Song.addSection(
            audioEngine->loadSound("audio/song2/level3.wav"),
            0,
            0,
            audioEngine->loadSound("audio/song2/level3outro.wav")
        );

        stage1Song.addSection(
            audioEngine->loadSound("audio/song2/level4.wav"),
            0,
            0,
            audioEngine->loadSound("audio/song2/level4outro.wav")
        );

        stage1Song.addSection(
            audioEngine->loadSound("audio/song2/level5.wav"),0,0,0
        );

        stage1Song.addSection(
            audioEngine->loadSound("audio/song2/level6.wav"),0,0,0
        );
        
        stage1Song.addSection(
            audioEngine->loadSound("audio/song2/level7.wav"),
            0,
            0,
            audioEngine->loadSound("audio/song2/level7outro.wav")
        );

        stage1Song.addSection(
            audioEngine->loadSound("audio/song2/level8.wav"),
            audioEngine->loadSound("audio/song2/level8fill.wav"),
            audioEngine->loadSound("audio/song2/level8variation.wav"),
            0
        );

        stage1Song.addSection(
            audioEngine->loadSound("audio/song2/level9.wav"),
            0,
            audioEngine->loadSound("audio/song2/level9variation.wav"),
            audioEngine->loadSound("audio/song2/level9outro.wav")
        );

        stage1Song.addSection(
            audioEngine->loadSound("audio/song2/level10.wav"),
            0,
            audioEngine->loadSound("audio/song2/level10variation.wav"),
            audioEngine->loadSound("audio/song2/level10outro.wav")
        );

        stage1Song.addSection(
            audioEngine->loadSound("audio/song2/level11.wav"),
            audioEngine->loadSound("audio/song2/level11fill.wav"),
            audioEngine->loadSound("audio/song2/level11variation.wav"),
            0
        );

        stage1Song.addSection(
            audioEngine->loadSound("audio/song2/level12.wav"),
            0,
            audioEngine->loadSound("audio/song2/level12variation.wav"),
            0
        );

        stage1Song.addSection(
            audioEngine->loadSound("audio/song2/level13.wav"),
            audioEngine->loadSound("audio/song2/level13fill.wav"),
            audioEngine->loadSound("audio/song2/level13variation.wav"),
            0
        );

        stage1Song.addSection(
            audioEngine->loadSound("audio/song2/level14.wav"),
            0,
            0,
            audioEngine->loadSound("audio/song2/level14outro.wav")
        );

        stage1Song.addSection(
            audioEngine->loadSound("audio/song2/level15.wav"),
            0,
            audioEngine->loadSound("audio/song2/level15variation.wav"),
            0
        );
        prototypes[Song::Stage1] = stage1Song;
    }

    DynamicSong create(Song song) {
        if (prototypes.find(song) != prototypes.end()) {
            return prototypes[song].clone();
        }

        std::cerr << "unknown Song" << std::endl;
        return DynamicSong();
    }

private:
    std::unordered_map<Song, DynamicSong> prototypes;

};
