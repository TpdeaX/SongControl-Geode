#pragma once
#include <Geode/Geode.hpp>
#include <Geode/Bindings.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include "Utils.hpp"

using namespace geode::prelude;

class SliderSongControl : public cocos2d::CCNode {
public:
	static SliderSongControl* create(SongInfoObject* song);
	bool init(SongInfoObject* song);
	void update(float dt);
	void musicSliderChanged(cocos2d::CCObject* pSender);
	void createSlider();
	void createLabels();
	cocos2d::CCLabelBMFont* createLabel(const char* text, const char* font, float scale, const cocos2d::CCPoint& position, const cocos2d::CCPoint& anchor);
	unsigned int getSongLength(bool currentSong);
	void resetUI();
	void updateSliderAndTimeLabels(unsigned int currentPosition, unsigned int totalDuration);
	void tryAutoPlayMusic();

	SongInfoObject* m_pSongObj = nullptr;
	Slider* m_pSliderMusic = nullptr;
	cocos2d::CCLabelBMFont* m_pCurrentTimeSong = nullptr;
	cocos2d::CCLabelBMFont* m_pStartTimeSong = nullptr;
	cocos2d::CCLabelBMFont* m_pEndTimeSong = nullptr;
	bool onChanged = false;
};

class SongControlLayer : public Popup<GJSongBrowser*, int> {
public:
	static bool isGameShow;
	static SongControlLayer* create(GJSongBrowser* parent, int i);
	bool setup(GJSongBrowser* ref, int i) override;
	virtual void keyBackClicked();
	void keyDown(cocos2d::enumKeyCodes key);
	void onPrevSong(cocos2d::CCObject* pSender);
	void onNextSong(cocos2d::CCObject* pSender);
	void onUseSong(cocos2d::CCObject* pSender);
	void onPauseSong(cocos2d::CCObject* pSender);
	void onDeleteSong(cocos2d::CCObject* pSender);
	void changeToSongAtIndex(int index);
	void updateButtonUse();
	void updateButtonPause();
	void DeleteMusic(cocos2d::CCObject* pSender);
	void onSettings(cocos2d::CCObject* pSender);

	GJSongBrowser* m_pParentA = nullptr;
	cocos2d::extension::CCScale9Sprite* m_pBG = nullptr;
	CustomSongWidget* m_CSWCurrent = nullptr;
	SliderSongControl* m_pSliderMusic = nullptr;
	ButtonSprite* imageButton7_ = nullptr;
	ButtonSprite* imageButton6_ = nullptr;
	CCMenuItemSpriteExtra* buttonUse = nullptr;
	cocos2d::CCLabelBMFont* m_lNumberSongTotal = nullptr;
	int m_iPag = 0;
	int m_iIndex = 0;
};
