#include "SongControlPopup.hpp"

bool SongControlLayer::isGameShow = false;

SongControlLayer* SongControlLayer::create(GJSongBrowser* parent, int i) {
	auto ret = new SongControlLayer();

	if (ret && ret->init(Utils::WinSize().width * 2.f, Utils::WinSize().height / 2.f, parent, i)) {
		SongControlLayer::isGameShow = true;
		ret->autorelease();
		return ret;
	}

	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool SongControlLayer::setup(GJSongBrowser* parent, int i) {

	m_noElasticity = true;

	this->m_iPag = i;
	this->m_pParentA = parent;

	this->m_bgSprite->setPosition(0, 80.f);
	this->m_closeBtn->setVisible(false);

	auto m_pButtonsMenu = cocos2d::CCMenu::create();
	m_mainLayer->addChild(m_pButtonsMenu);

	auto spriteButton = cocos2d::CCSprite::createWithSpriteFrameName("edit_leftBtn2_001.png");
	auto imageButton = ButtonSprite::create("", 30, true, "bigFont.fnt", "GJ_button_01.png", 35, 0.5f);
	imageButton->addChild(spriteButton);
	spriteButton->setPosition({ 20.f, 18.f });
	imageButton->setScale(0.8f);
	auto btnPrevSong = CCMenuItemSpriteExtra::create(imageButton, this, (cocos2d::SEL_MenuHandler)&SongControlLayer::onPrevSong);//(cocos2d::SEL_MenuHandler)&ButtonsClass::onSongControlLayer);
	btnPrevSong->setPosition({ -100.f, -40.f });
	m_pButtonsMenu->addChild(btnPrevSong);

	auto spriteButton_2 = cocos2d::CCSprite::createWithSpriteFrameName("edit_rightBtn2_001.png");
	auto imageButton_2 = ButtonSprite::create("", 30, true, "bigFont.fnt", "GJ_button_01.png", 35, 0.5f);
	spriteButton_2->setPosition({ 20.f, 18.f });
	imageButton_2->addChild(spriteButton_2);
	imageButton_2->setScale(0.8f);
	auto btnNextSong = CCMenuItemSpriteExtra::create(imageButton_2, this, (cocos2d::SEL_MenuHandler)&SongControlLayer::onNextSong);//(cocos2d::SEL_MenuHandler)&ButtonsClass::onSongControlLayer);
	btnNextSong->setPosition({ -50.f, -40.f });
	m_pButtonsMenu->addChild(btnNextSong);

	auto prevImage = cocos2d::CCSprite::createWithSpriteFrameName("edit_leftBtn2_001.png");
	prevImage->setScale(0.5f);
	prevImage->setPosition({ Utils::WinSize().width / 2 - 110.f, 80.f });
	auto prevImageText = cocos2d::CCLabelBMFont::create("Prev. Song", "bigFont.fnt");
	prevImageText->setScale(0.3f);
	prevImageText->setPosition({ Utils::WinSize().width / 2 - 95.f, 80.f });
	prevImageText->setAnchorPoint({ 0.f, 0.5f });
	auto nextImage = cocos2d::CCSprite::createWithSpriteFrameName("edit_rightBtn2_001.png");
	nextImage->setScale(0.5f);
	nextImage->setPosition({ Utils::WinSize().width / 2 - 109.f, 60.f });
	auto nextImageText = cocos2d::CCLabelBMFont::create("Next Song", "bigFont.fnt");
	nextImageText->setScale(0.3f);
	nextImageText->setPosition({ Utils::WinSize().width / 2 - 95.f, 60.f });
	nextImageText->setAnchorPoint({ 0.f, 0.5f });
	m_mainLayer->addChild(prevImage);
	m_mainLayer->addChild(nextImage);
	m_mainLayer->addChild(prevImageText);
	m_mainLayer->addChild(nextImageText);

	auto downloadedSongs = MusicDownloadManager::sharedState()->getDownloadedSongs();
	this->m_iIndex = 10 * this->m_iPag;

	for (size_t i = 0; i < downloadedSongs->count(); i++) {
		auto musicI = reinterpret_cast<SongInfoObject*>(downloadedSongs->objectAtIndex(i));
		if (Utils::shareFMOD()->isMusicPlaying(MusicDownloadManager::sharedState()->pathForSong(musicI->m_songID), 0)) {
			this->m_iIndex = i;
			break;
		}
	}

	changeToSongAtIndex(this->m_iIndex);

	auto imageButton5_ = ButtonSprite::create("Delete", 80.f, false, "bigFont.fnt", "GJ_button_06.png", 35.f, 0.6f);
	imageButton5_->setScale(0.7f);
	auto buttonDelete = CCMenuItemSpriteExtra::create(imageButton5_, this, (cocos2d::SEL_MenuHandler)&SongControlLayer::onDeleteSong);//(cocos2d::SEL_MenuHandler)&MusicOffsetSetupLayer::onStop);
	buttonDelete->setPosition({ 40.f - Utils::WinSize().width, -100.f });
	m_pButtonsMenu->addChild(buttonDelete);

	imageButton6_ = ButtonSprite::create("Pause", 70.f, true, "bigFont.fnt", "GJ_button_02.png", 35.f, 0.6f);
	imageButton6_->setScale(0.7f);
	auto buttonPause = CCMenuItemSpriteExtra::create(imageButton6_, this, (cocos2d::SEL_MenuHandler)&SongControlLayer::onPauseSong);//(cocos2d::SEL_MenuHandler)&MusicOffsetSetupLayer::onStop);
	buttonPause->setPosition({ 40.f - Utils::WinSize().width, -70.f });
	m_pButtonsMenu->addChild(buttonPause);

	updateButtonPause();

	imageButton7_ = ButtonSprite::create("Use", 70.f, true, "bigFont.fnt", "GJ_button_03.png", 35.f, 0.6f);
	imageButton7_->setScale(0.7f);
	buttonUse = CCMenuItemSpriteExtra::create(imageButton7_, this, (cocos2d::SEL_MenuHandler)&SongControlLayer::onUseSong);//(cocos2d::SEL_MenuHandler)&MusicOffsetSetupLayer::onStop);
	buttonUse->setPosition({ 40.f - Utils::WinSize().width, -40.f });
	m_pButtonsMenu->addChild(buttonUse);

	buttonUse->setVisible(Utils::shareManager()->getEditorLayer());

	updateButtonUse();
	/*
	auto settingsButtonImage = cocos2d::CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
	settingsButtonImage->setScale(0.6f);
	auto settingsButton = CCMenuItemSpriteExtra::create(settingsButtonImage, this, (cocos2d::SEL_MenuHandler)&SongControlLayer::onSettings); //(cocos2d::SEL_MenuHandler)&BetterPause::onOptionsLayer);
	settingsButton->setPosition({ 104.f - Utils::WinSize().width, -70.f });
	m_pButtonsMenu->addChild(settingsButton);
	*/

	m_lNumberSongTotal = cocos2d::CCLabelBMFont::create(cocos2d::CCString::createWithFormat(
		"Sound %i of %i", this->m_iIndex + 1, downloadedSongs->count())->getCString(), "goldFont.fnt");
	m_lNumberSongTotal->setScale(0.5f);
	m_lNumberSongTotal->setPositionY(148.f);
	m_mainLayer->addChild(m_lNumberSongTotal);

	auto animationEntry = cocos2d::CCEaseElasticOut::create((cocos2d::CCActionInterval*)cocos2d::CCMoveTo::create(0.5f, { Utils::WinSize().width / 2.f, -30.f }), 1.f);
	m_mainLayer->runAction(cocos2d::CCSequence::create((cocos2d::CCFiniteTimeAction*)animationEntry, nullptr));



	this->setKeypadEnabled(true);
	this->setTouchEnabled(true);
	this->setKeyboardEnabled(true);
	this->setMouseEnabled(true);

	return true;
}

void SongControlLayer::keyBackClicked() {
	SongControlLayer::isGameShow = false;
	FLAlertLayer::keyBackClicked();
}

void SongControlLayer::keyDown(cocos2d::enumKeyCodes key) {
	switch (key) {
	case cocos2d::enumKeyCodes::KEY_Space:
		break;
	case cocos2d::enumKeyCodes::CONTROLLER_X:
		this->keyBackClicked();
		break;
	case cocos2d::enumKeyCodes::KEY_Left:
	case cocos2d::enumKeyCodes::CONTROLLER_Left:
		break;
	case cocos2d::enumKeyCodes::KEY_Right:
	case cocos2d::enumKeyCodes::CONTROLLER_Right:
		break;
	default:
		CCLayer::keyDown(key);
	}
}

void SongControlLayer::changeToSongAtIndex(int index) {
	auto downloadedSongs = MusicDownloadManager::sharedState()->getDownloadedSongs();
	downloadedSongs->retain();
	if (index < 0) {
		index = m_iIndex = downloadedSongs->count() - 1;
	}
	else if (index > (downloadedSongs->count() - 1)) {
		index = m_iIndex = 0;
	}
	else if (downloadedSongs->count() == 0) {
		index = m_iIndex = -1;
	}

	if (m_lNumberSongTotal) {
		m_lNumberSongTotal->setString(cocos2d::CCString::createWithFormat(
			"Sound %i of %i", this->m_iIndex + 1, downloadedSongs->count())->getCString());
	}


	auto musicSelect = downloadedSongs->count() == 0 ? 0 : reinterpret_cast<SongInfoObject*>(downloadedSongs->objectAtIndex(index));

	if (m_CSWCurrent) {
		m_CSWCurrent->removeFromParentAndCleanup(true);
	}

	CustomSongDelegate* csd = nullptr;

	if (this->m_pParentA) {
		for (size_t i = 0; i < CCScene::get()->getChildrenCount(); i++)
		{
			auto layer = exact_cast<CustomSongLayer*>(CCScene::get()->getChildren()->objectAtIndex(i));
			if (layer) {
				csd = Utils::from<CustomSongDelegate*>(layer, 0x1fc);

			}
		}
	}


	m_CSWCurrent = CustomSongWidget::create(musicSelect, csd,
		Utils::shareManager()->getEditorLayer() == nullptr ? 0 : Utils::shareManager()->getEditorLayer()->m_levelSettings, true, 0, false, 0, 0);
	m_CSWCurrent->setPositionY(90.f);
	m_CSWCurrent->m_songIDLabel->setString(cocos2d::CCString::createWithFormat(
		"SongID: %i\nSize: %.2fMB", (m_CSWCurrent->m_songInfoObject ? m_CSWCurrent->m_songInfoObject->m_songID : 0), (m_CSWCurrent->m_songInfoObject ? Utils::fileSizeMB(MusicDownloadManager::sharedState()->pathForSong(this->m_CSWCurrent->m_songInfoObject->m_songID).c_str()) : 0))->getCString());
	m_CSWCurrent->m_songIDLabel->setScale(0.3f);
	m_CSWCurrent->m_songIDLabel->setPosition({ -138.f, -25.f });
	m_CSWCurrent->m_songIDLabel->setAnchorPoint({ 0.f, 0.5f });
	if (m_CSWCurrent->m_selectSongBtn) {
		m_CSWCurrent->m_selectSongBtn->setVisible(false);
	}
	m_mainLayer->addChild(m_CSWCurrent);

	m_pSliderMusic = SliderSongControl::create(musicSelect);
	m_pSliderMusic->setScale(0.7f);
	m_pSliderMusic->setPosition({ 44.f, -43.f });
	m_CSWCurrent->addChild(m_pSliderMusic);

	updateButtonUse();
	updateButtonPause();
	downloadedSongs->release();
}

void SongControlLayer::onPrevSong(cocos2d::CCObject* pSender) {
	if (!this->m_CSWCurrent->m_songInfoObject) {
		return;
	}
	this->m_iIndex--;
	this->changeToSongAtIndex(this->m_iIndex);
	if (Mod::get()->getSettingValue<bool>("auto-play-on-song-change") && this->m_CSWCurrent->m_playbackBtn) {
		this->m_CSWCurrent->m_playbackBtn->activate();
	}
}

void SongControlLayer::onNextSong(cocos2d::CCObject* pSender) {
	if (!this->m_CSWCurrent->m_songInfoObject) {
		return;
	}
	this->m_iIndex++;
	this->changeToSongAtIndex(this->m_iIndex);
	if (Mod::get()->getSettingValue<bool>("auto-play-on-song-change") && this->m_CSWCurrent->m_playbackBtn) {
		this->m_CSWCurrent->m_playbackBtn->activate();
	}
}

void SongControlLayer::updateButtonUse() {
	if (!LevelEditorLayer::get() || !m_CSWCurrent->m_songInfoObject) {
		return;
	}

	std::string currentID = std::to_string((int)m_CSWCurrent->m_songInfoObject->m_songID);

	std::stringstream ss(LevelEditorLayer::get()->m_levelSettings->m_level->m_songIDs);
	std::vector<std::string> ids;
	std::string id;
	while (std::getline(ss, id, ',')) {
		ids.push_back(id);
	}

	bool isUseMusic = false;
	for (const auto& songID : ids) {
		if (songID == currentID) {
			isUseMusic = true;
			break;
		}
	}

	if (imageButton7_) {
		imageButton7_->updateBGImage(isUseMusic ? "GJ_button_04.png" : "GJ_button_03.png");
		imageButton7_->m_label->setString(isUseMusic ? "In Use" : "Use");
	}
	if (buttonUse) {
		buttonUse->setEnabled(!isUseMusic);
	}
}

void SongControlLayer::onUseSong(cocos2d::CCObject* pSender) {

	if (!m_CSWCurrent->m_songInfoObject) {
		return;
	}

	m_CSWCurrent->onSelect(pSender);
	updateButtonUse();
	m_CSWCurrent->m_songIDLabel->setString(cocos2d::CCString::createWithFormat(
		"SongID: %i\nSize: %.2fMB", m_CSWCurrent->m_songInfoObject->m_songID, m_CSWCurrent->m_songInfoObject->m_fileSize)->getCString());
	m_CSWCurrent->m_songIDLabel->setScale(0.3f);
	m_CSWCurrent->m_songIDLabel->setPosition({ -140.f, -25.f });
	m_CSWCurrent->m_songIDLabel->setAnchorPoint({ 0.f, 0.5f });
	if (this->m_pParentA) {
		this->m_pParentA->loadPage(m_pParentA->m_page);
	}
}

void SongControlLayer::updateButtonPause() {
	if (!m_CSWCurrent->m_songInfoObject) {
		return;
	}


	bool isPaused = false;

	Utils::shareFMOD()->getActiveMusic(0)->getPaused(&isPaused);

	if (imageButton6_) {
		if (isPaused) {
			imageButton6_->updateBGImage("GJ_button_01.png");
			imageButton6_->m_label->setString("Resume");
		}
		else {
			imageButton6_->updateBGImage("GJ_button_02.png");
			imageButton6_->m_label->setString("Pause");
		}
	}

	return;


	if (imageButton6_) {
		imageButton6_->updateBGImage("GJ_button_02.png");
		imageButton6_->m_label->setString("Pause");
	}
}

void SongControlLayer::onPauseSong(cocos2d::CCObject* pSender) {

	if (!m_CSWCurrent->m_songInfoObject) {
		return;
	}


	bool isPaused = false;

	Utils::shareFMOD()->getActiveMusic(0)->getPaused(&isPaused);
	Utils::shareFMOD()->getActiveMusic(0)->setPaused(!isPaused);


	updateButtonPause();

}

void SongControlLayer::DeleteMusic(cocos2d::CCObject* pSender) {
	if (this->m_CSWCurrent) {
		this->m_CSWCurrent->deleteSong();
	}

	if (this->m_pParentA) {
		this->m_pParentA->loadPage(this->m_pParentA->m_page);
	}

	SongControlLayer::isGameShow = true;
	auto scl = SongControlLayer::create(this->m_pParentA ? this->m_pParentA : 0, this->m_pParentA ? this->m_pParentA->m_page : 0);
	Utils::shareDirectorA()->getRunningScene()->addChild(scl, Utils::shareDirectorA()->getRunningScene()->getHighestChildZ());
	scl->m_iIndex = this->m_iIndex;
	scl->changeToSongAtIndex(scl->m_iIndex);
	this->removeFromParentAndCleanup(true);

}

void SongControlLayer::onDeleteSong(cocos2d::CCObject* pSender) {
	if (!m_CSWCurrent->m_songInfoObject) {
		return;
	}

	FMODAudioEngine::sharedEngine()->stopAllMusic();
	GameManager::get()->fadeInMenuMusic();

	geode::createQuickPopup(
		"Delete Song",
		"Do you want to <cr>Delete</c> this song?",
		"Cancel", "Delete",
		[this, pSender](FLAlertLayer* fla, bool btn2) {
			if (btn2) {
				this->DeleteMusic(pSender);
			}
		}
	);
}

SliderSongControl* SliderSongControl::create(SongInfoObject* song) {
	auto ret = new SliderSongControl();

	if (ret && ret->init(song)) {
		ret->autorelease();
		ret->scheduleUpdate();
		return ret;
	}

	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool SliderSongControl::init(SongInfoObject* song) {
	if (!cocos2d::CCNode::init()) {
		return false;
	}

	this->m_pSongObj = song;

	createSlider();
	createLabels();

	return true;
}

void SliderSongControl::createSlider() {
	m_pSliderMusic = Slider::create(this, (cocos2d::SEL_MenuHandler)&SliderSongControl::musicSliderChanged, 1.f);
	m_pSliderMusic->setValue(0.f);
	m_pSliderMusic->updateBar();
	m_pSliderMusic->setScale(0.8f);
	m_pSliderMusic->setAnchorPoint({ 0.f, 0.5f });
	this->addChild(m_pSliderMusic);
}

void SliderSongControl::createLabels() {

	auto showMusicTime = Mod::get()->getSettingValue<bool>("show-position-music") ? "%.2f" : "%02d:%02d";
	auto showMusicTimeWF = Mod::get()->getSettingValue<bool>("show-position-music") ? "0.00" : "00:00";

	m_pCurrentTimeSong = createLabel(showMusicTimeWF, "goldFont.fnt", 0.5f, { 0.f, 15.f }, { 0.f, 0.5f });
	m_pStartTimeSong = createLabel(showMusicTimeWF, "bigFont.fnt", 0.4f, { -90.f, 32.f }, { 1.f, 0.5f });

	unsigned int length = getSongLength(false);
	unsigned int minutes = length / 60000;
	unsigned int seconds = (length % 60000) / 1000;

	if (Mod::get()->getSettingValue<bool>("show-position-music")) {
		m_pEndTimeSong = createLabel(cocos2d::CCString::createWithFormat(showMusicTime, length / 1000.f)->getCString(), "bigFont.fnt", 0.4f, { 90.f, 32.f }, { 0.f, 0.5f });
	}
	else {
		m_pEndTimeSong = createLabel(cocos2d::CCString::createWithFormat(showMusicTime, minutes, seconds)->getCString(), "bigFont.fnt", 0.4f, { 90.f, 32.f }, { 0.f, 0.5f });

	}
}

cocos2d::CCLabelBMFont* SliderSongControl::createLabel(const char* text, const char* font, float scale, const cocos2d::CCPoint& position, const cocos2d::CCPoint& anchor) {
	auto label = cocos2d::CCLabelBMFont::create(text, font);
	label->setScale(scale);
	label->setPosition(position);
	label->setAnchorPoint(anchor);
	this->addChild(label);
	return label;
}

unsigned int SliderSongControl::getSongLength(bool currentSong) {
	if (currentSong && Utils::shareFMOD()->m_sound) {
		return Utils::shareFMOD()->getMusicLengthMS(0);
	}

	if (!this->m_pSongObj) {
		return 0;
	}

	FMOD::Sound* sound = nullptr;

	if (!currentSong) {
		Utils::shareFMOD()->m_system->createSound(MusicDownloadManager::sharedState()->pathForSong(this->m_pSongObj->m_songID).c_str(), FMOD_DEFAULT, nullptr, &sound);
	}

	unsigned int length = 0;

	if (sound) {
		sound->getLength(&length, FMOD_TIMEUNIT_MS);
		sound->release();
		CC_SAFE_DELETE(sound);
	}

	return length;
}



void SliderSongControl::musicSliderChanged(cocos2d::CCObject* pSender) {

	if (!this->m_pSongObj) {
		return;
	}

	if (!Utils::shareFMOD()->isMusicPlaying(MusicDownloadManager::sharedState()->pathForSong(this->m_pSongObj->m_songID), 0)) {
		resetUI();
		return;
	}

	this->onChanged = true;

	unsigned int totalDuration = getSongLength(true);

	auto valueSlider = this->m_pSliderMusic->getValue();

	FMODAudioEngine::sharedEngine()->setMusicTimeMS(static_cast<unsigned int>(totalDuration * valueSlider), true, false);

	updateSliderAndTimeLabels(FMODAudioEngine::sharedEngine()->getMusicTimeMS(0), totalDuration);
}

void SliderSongControl::update(float dt) {
	if (this->onChanged) {
		this->onChanged = false;
		return;
	}

	if (!this->m_pSongObj || !Utils::shareFMOD()->isMusicPlaying(MusicDownloadManager::sharedState()->pathForSong(this->m_pSongObj->m_songID), 0)) {
		resetUI();
		return;
	}

	auto totalDuration = getSongLength(true);
	auto currentPosition = FMODAudioEngine::sharedEngine()->getMusicTimeMS(0);

	updateSliderAndTimeLabels(currentPosition, totalDuration);

	if (currentPosition >= totalDuration && Mod::get()->getSettingValue<bool>("auto-play-next-song")) {
		tryAutoPlayMusic();
		unscheduleUpdate();
	}

	auto tryAch = reinterpret_cast<SongControlLayer*>(this->getParent()->getParent()->getParent());
	if (tryAch) {
		tryAch->m_CSWCurrent->m_songIDLabel->setString(cocos2d::CCString::createWithFormat(
			"SongID: %i\nSize: %.2fMB", (tryAch->m_CSWCurrent->m_songInfoObject ? tryAch->m_CSWCurrent->m_songInfoObject->m_songID : 0), (tryAch->m_CSWCurrent->m_songInfoObject ? Utils::fileSizeMB(MusicDownloadManager::sharedState()->pathForSong(tryAch->m_CSWCurrent->m_songInfoObject->m_songID).c_str()) : 0))->getCString());
	}
}

void SliderSongControl::tryAutoPlayMusic() {
	auto tryAch = reinterpret_cast<SongControlLayer*>(this->getParent()->getParent()->getParent());
	tryAch->m_iIndex++;
	tryAch->changeToSongAtIndex(tryAch->m_iIndex);
	tryAch->m_CSWCurrent->m_playbackBtn->activate();
}

void SliderSongControl::resetUI() {
	m_pSliderMusic->setValue(0.f);
	m_pSliderMusic->updateBar();
	m_pCurrentTimeSong->setString(Mod::get()->getSettingValue<bool>("show-position-music") ? "0.00" : "00:00");
	m_pCurrentTimeSong->setPositionX(m_pSliderMusic->getThumb()->getPositionX());
	m_pCurrentTimeSong->setAnchorPoint({ 0.f, 0.5f });
}

void SliderSongControl::updateSliderAndTimeLabels(unsigned int currentPosition, unsigned int totalDuration) {
	float valueV = static_cast<float>(currentPosition) / static_cast<float>(totalDuration);

	m_pSliderMusic->setValue(valueV);
	m_pSliderMusic->updateBar();

	int currentMinutes = static_cast<int>(currentPosition / 1000.f / 60.f);
	int currentSeconds = static_cast<int>(currentPosition / 1000.f) % 60;
	int totalMinutes = static_cast<int>(totalDuration / 1000.f / 60.f);
	int totalSeconds = static_cast<int>(totalDuration / 1000.f) % 60;

	auto showMusicTime = Mod::get()->getSettingValue<bool>("show-position-music") ? "%.2f" : "%02d:%02d";

	if (Mod::get()->getSettingValue<bool>("show-position-music")) {
		m_pCurrentTimeSong->setString(cocos2d::CCString::createWithFormat(
			showMusicTime, currentPosition / 1000.f)->getCString());
	}
	else {
		m_pCurrentTimeSong->setString(cocos2d::CCString::createWithFormat(
			showMusicTime, currentMinutes, currentSeconds, totalMinutes, totalSeconds)->getCString());
	}

	m_pCurrentTimeSong->setPositionX(m_pSliderMusic->getThumb()->getPositionX());
	m_pCurrentTimeSong->setAnchorPoint({ valueV, 0.5f });

	std::cout << currentPosition << "\t" << totalDuration << std::endl;
}