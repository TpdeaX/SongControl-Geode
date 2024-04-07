#include "SearchMusicPopup.hpp"

kTypeSearchMusicLayer SearchMusicLayer::typeSearch = kTypeSearchMusicLayer::kTypeSearchNone;
std::string SearchMusicLayer::stringToSearch = "";

SearchMusicLayer* SearchMusicLayer::create(GJSongBrowser* parent) {
	auto ret = new SearchMusicLayer();

	if (ret && ret->init(Utils::WinSize().width / 2.f + 70.f, Utils::WinSize().height / 2.f, parent, "GJ_square05.png")) {
		ret->autorelease();
		return ret;
	}

	CC_SAFE_DELETE(ret);
	return nullptr;
}
bool SearchMusicLayer::setup(GJSongBrowser* parent) {

	this->m_pParentA = parent;
	this->m_noElasticity = true;

	m_TitleLabel = cocos2d::CCLabelBMFont::create("Search Song", "bigFont.fnt");
	m_TitleLabel->setPosition({ Utils::WinSize().width / 2.f,Utils::WinSize().height / 2.f + 54.f });
	m_TitleLabel->setScale(0.8f);
	m_mainLayer->addChild(m_TitleLabel);

	m_pDescriptionLabel = cocos2d::CCLabelBMFont::create("Enter the name of the song or\nthe name of the author", "goldFont.fnt");
	m_pDescriptionLabel->setAlignment(cocos2d::kCCTextAlignmentCenter);
	m_pDescriptionLabel->setPosition({ Utils::WinSize().width / 2.f,Utils::WinSize().height / 2.f + 18.f });
	m_pDescriptionLabel->setScale(0.5f);
	m_mainLayer->addChild(m_pDescriptionLabel);

	m_pBGTextInput = cocos2d::extension::CCScale9Sprite::create("square02_small.png");
	m_pBGTextInput->setContentSize({ 210.f, 35.f });
	m_pBGTextInput->setPosition({ Utils::WinSize().width / 2.f - 54.5f,Utils::WinSize().height / 2.f - 34.f });
	m_pBGTextInput->setOpacity(static_cast<GLubyte>(50));
	m_mainLayer->addChild(m_pBGTextInput);

	m_pBGTextInput_2 = cocos2d::extension::CCScale9Sprite::create("square02_small.png");
	m_pBGTextInput_2->setContentSize({ 255.f, 35.f });
	m_pBGTextInput_2->setScale(0.8f);
	m_pBGTextInput_2->setPosition({ Utils::WinSize().width / 2.f - 54.5f,Utils::WinSize().height / 2.f - 34.f });
	m_pBGTextInput_2->setOpacity(static_cast<GLubyte>(50));
	m_mainLayer->addChild(m_pBGTextInput_2);

	m_pTextInput = CCTextInputNode::create(190.f, 20.f, "Search", "bigFont.fnt");
	m_pTextInput->setLabelPlaceholderColor({ 0x75, 0xAA, 0xF0 });
	m_pTextInput->setMaxLabelScale(0.7f);
	m_pTextInput->setPosition({ -41.f, -14.f });
	m_pTextInput->setDelegate(this);
	m_pTextInput->setPosition({ Utils::WinSize().width / 2.f - 54.5f,Utils::WinSize().height / 2.f - 34.f });
	m_mainLayer->addChild(m_pTextInput);

	m_pMenuButtons = cocos2d::CCMenu::create();
	m_pMenuButtons->setPosition({ 0.f, 0.f });
	m_mainLayer->addChild(m_pMenuButtons);

	auto buttonSpriteSearchMusic = ButtonSprite::create("Search", 0xFC, false, "goldFont.fnt", "GJ_button_04.png", 30.f, 0.6f);
	buttonSpriteSearchMusic->setScale(0.8f);
	m_pSearchMusicName = CCMenuItemSpriteExtra::create(buttonSpriteSearchMusic, this, (cocos2d::SEL_MenuHandler)&SearchMusicLayer::onSearchMusic);
	m_pSearchMusicName->setPosition({ Utils::WinSize().width / 2.f + 90.5f, Utils::WinSize().height / 2.f - 34.f });
	m_pMenuButtons->addChild(m_pSearchMusicName);

	auto spriteSearchArtistName = cocos2d::CCSprite::createWithSpriteFrameName("GJ_longBtn05_001.png");
	spriteSearchArtistName->setScale(0.8f);
	m_pSearchArtistName = CCMenuItemSpriteExtra::create(spriteSearchArtistName, this, (cocos2d::SEL_MenuHandler)&SearchMusicLayer::onSearchArtist);
	m_pSearchArtistName->setPosition({ Utils::WinSize().width / 2.f + 145.5f, Utils::WinSize().height / 2.f - 34.f });
	m_pMenuButtons->addChild(m_pSearchArtistName);
	/*
	auto settingsButtonImage = cocos2d::CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
	settingsButtonImage->setScale(0.6f);
	auto settingsButton = CCMenuItemSpriteExtra::create(settingsButtonImage, this, (cocos2d::SEL_MenuHandler)&SearchMusicLayer::onSettings); //(cocos2d::SEL_MenuHandler)&BetterPause::onOptionsLayer);
	settingsButton->setPosition({ Utils::WinSize().width / 2.f + 145.5f, Utils::WinSize().height / 2.f + 20.f });
	m_pMenuButtons->addChild(settingsButton);
	*/
	auto deleteTextButtonImage = cocos2d::CCSprite::createWithSpriteFrameName("GJ_resetBtn_001.png");
	auto deleteTextButton = CCMenuItemSpriteExtra::create(deleteTextButtonImage, this, (cocos2d::SEL_MenuHandler)&SearchMusicLayer::onDeleteTextSearch); //(cocos2d::SEL_MenuHandler)&BetterPause::onOptionsLayer);
	deleteTextButton->setPosition({ Utils::WinSize().width / 2.f + 145.5f, Utils::WinSize().height / 2.f + 54.5f });
	m_pMenuButtons->addChild(deleteTextButton);

	this->setKeypadEnabled(true);
	this->setTouchEnabled(true);
	this->setKeyboardEnabled(true);
	this->setMouseEnabled(true);

	return true;
}

void SearchMusicLayer::onSettings(cocos2d::CCObject* pSender) {
	
}

void SearchMusicLayer::onDeleteTextSearch(cocos2d::CCObject* pSender) {
	this->m_pTextInput->setString("");
	this->m_pTextInput->onClickTrackNode(false);
}

void SearchMusicLayer::updateParentAContent() {
	if (this->m_pParentA) {
		auto parentAArray = Utils::from<cocos2d::CCArray*>(this->m_pParentA, 0x208);
		parentAArray->removeAllObjects();
		parentAArray->initWithArray(MusicDownloadManager::sharedState()->getDownloadedSongs());
		parentAArray->retain();
		this->m_pParentA->loadPage(0);

		auto menuButtonsCreateExt = reinterpret_cast<cocos2d::CCMenu*>(this->m_pParentA->m_mainLayer->getChildByID("menu-buttons-song-control"));
		auto searchButton = reinterpret_cast<CCMenuItemSpriteExtra*>(menuButtonsCreateExt->getChildByID("button-song-findOff-control"));
		searchButton->setVisible(!stringToSearch.empty());
	}
}


void SearchMusicLayer::keyBackClicked() {
	/*this->m_pParentA->hideLayer(true);
	this->m_pParentA = GJSongBrowser::create(Utils::shareManager()->getEditorLayer() ?
		Utils::shareManager()->getEditorLayer()->m_pLevelSettings : 0);
	cocos2d::CCDirector::sharedDirector()
		->getRunningScene()
		->addChild(this->m_pParentA, cocos2d::CCDirector::sharedDirector()->getRunningScene()->getHighestChildZ());
	this->m_pParentA->showLayer(true);*/
	updateParentAContent();

	FLAlertLayer::keyBackClicked();
}

void SearchMusicLayer::keyDown(cocos2d::enumKeyCodes key) {
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

void SearchMusicLayer::textChanged(CCTextInputNode* input) {
	if (!Mod::get()->getSettingValue<bool>("live-search")) {
		return;
	}
	SearchMusicLayer::typeSearch = kTypeSearchMusicLayer::kTypeSearchSongName;
	SearchMusicLayer::stringToSearch = m_pTextInput->getString();

	updateParentAContent();
}

void SearchMusicLayer::onClose(cocos2d::CCObject* pSender) {
	this->keyBackClicked();
}

void SearchMusicLayer::onSearchMusic(cocos2d::CCObject* pSender) {
	SearchMusicLayer::typeSearch = kTypeSearchMusicLayer::kTypeSearchSongName;
	SearchMusicLayer::stringToSearch = m_pTextInput->getString();
	updateParentAContent();
	this->keyBackClicked();
}

void SearchMusicLayer::onSearchArtist(cocos2d::CCObject* pSender) {
	SearchMusicLayer::typeSearch = kTypeSearchMusicLayer::kTypeSearchArtistName;
	SearchMusicLayer::stringToSearch = m_pTextInput->getString();
	updateParentAContent();
	this->keyBackClicked();
}
