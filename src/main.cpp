#include <Geode/Geode.hpp>
#include <Geode/modify/GJSongBrowser.hpp>
#include <Geode/modify/CustomSongWidget.hpp>
#include <Geode/modify/MusicDownloadManager.hpp>
#include <Geode/modify/GJDropDownLayer.hpp>
#include <Geode/modify/CCScene.hpp>
#include <Geode/modify/CCNode.hpp>
#include "Utils.hpp"
#include "SearchMusicPopup.hpp"
#include "SongControlPopup.hpp"
#include "Keybinds.hpp"

using namespace geode::prelude;


class $modify(GJSongBrowserHook, GJSongBrowser) {
	void customSetup() {
		GJSongBrowser::customSetup();
		auto menu = cocos2d::CCMenu::create();
		menu->setPosition({ 0.f, 0.f });
		menu->setID("menu-buttons-song-control");
		this->m_mainLayer->addChild(menu);

		auto openinfoLayerbutton_sprite = ButtonSprite::createWithSpriteFrameName("GJ_audioOnBtn_001.png");
		openinfoLayerbutton_sprite->setScale(1.2f);
		auto openinfoLayerbutton = CCMenuItemSpriteExtra::create(openinfoLayerbutton_sprite, this, (cocos2d::SEL_MenuHandler)&GJSongBrowserHook::onSongControlLayer);
		openinfoLayerbutton->setPosition({ 30.f, 30.f });
		menu->addChild(openinfoLayerbutton);

		auto spriteFindMusic = cocos2d::CCSprite::createWithSpriteFrameName("gj_findBtn_001.png");
		auto openFindMusic = CCMenuItemSpriteExtra::create(spriteFindMusic, this, (cocos2d::SEL_MenuHandler)&GJSongBrowserHook::onSearchMusicLayer);
		openFindMusic->setPosition({ Utils::WinSize().width - 20.f, Utils::WinSize().height - 50.f });
		menu->addChild(openFindMusic);

		auto spriteFindOffMusic = cocos2d::CCSprite::createWithSpriteFrameName("gj_findBtnOff_001.png");
		auto findOffMusic = CCMenuItemSpriteExtra::create(spriteFindOffMusic, this, (cocos2d::SEL_MenuHandler)&GJSongBrowserHook::onSearchMusicOff);
		findOffMusic->setPosition({ Utils::WinSize().width - 20.f, Utils::WinSize().height - 90.f });
		findOffMusic->setID("button-song-findOff-control");
		findOffMusic->setVisible(false);
		menu->addChild(findOffMusic);
	}

	void onSearchMusicOff(cocos2d::CCObject* pSender) {
		SearchMusicLayer::typeSearch = kTypeSearchMusicLayer::kTypeSearchNone;
		SearchMusicLayer::stringToSearch = "";
		Utils::from<cocos2d::CCArray*>(this, 0x208)->removeAllObjects();
		Utils::from<cocos2d::CCArray*>(this, 0x208)->initWithArray(MusicDownloadManager::sharedState()->getDownloadedSongs());
		this->loadPage(0);
		if (this->m_mainLayer) {
			auto menuButtonsCreateExt = typeinfo_cast<cocos2d::CCMenu*>(this->m_mainLayer->getChildByID("menu-buttons-song-control"));
			if (menuButtonsCreateExt) {
				typeinfo_cast<CCMenuItemSpriteExtra*>(menuButtonsCreateExt->getChildByID("button-song-findOff-control"))->setVisible(false);
			}
		}
	}

	void onSongControlLayer(cocos2d::CCObject* pSender) {
		if (SongControlLayer::isGameShow) {
			return;
		}

		auto scl = SongControlLayer::create(this, this->m_page);
		scl->show();
		scl->setTag(Utils::shareDirectorA()->getRunningScene()->getHighestChildZ());
	}

	void onSearchMusicLayer(cocos2d::CCObject* pSender) {
		auto scl = SearchMusicLayer::create(this);
		scl->show();
		scl->setTag(Utils::shareDirectorA()->getRunningScene()->getHighestChildZ());
	}
};

class $modify(CustomSongWidget) {
	void onPlayback(cocos2d::CCObject * pSender) {
		CustomSongWidget::onPlayback(pSender);
		if (Utils::shareFMOD()->isMusicPlaying(
			MusicDownloadManager::sharedState()->pathForSong(this->m_uID),
			0
		)) {
			if (!Utils::shareManager()->getEditorLayer() &&
				Utils::shareManager()->getGameVariable("MMO") == false) {
				GameManager::get()->fadeInMenuMusic();
			}
		}
	}

	void updatePlaybackBtn() {
		CustomSongWidget::updatePlaybackBtn();
		for (size_t i = 0; i < this->getChildrenCount(); i++)
		{
			auto node = dynamic_cast<SliderSongControl*>(this->getChildren()->objectAtIndex(i));
			if (node) {
				this->m_songIDLabel->setString(cocos2d::CCString::createWithFormat(
					"SongID: %i\nSize: %.2fMB", this->m_songInfoObject->m_songID, this->m_songInfoObject->m_fileSize)->getCString());
				this->m_songIDLabel->setScale(0.3f);
				this->m_songIDLabel->setPosition({ -138.f, -25.f });
				this->m_songIDLabel->setAnchorPoint({ 0.f, 0.5f });
				break;
			}
		}
	}
};

class $modify(MusicDownloadManager) {
	cocos2d::CCArray* getDownloadedSongs() {
		auto ret = MusicDownloadManager::getDownloadedSongs();

		if (SearchMusicLayer::stringToSearch.empty() || SearchMusicLayer::typeSearch == kTypeSearchMusicLayer::kTypeSearchNone) {
			SearchMusicLayer::typeSearch = kTypeSearchMusicLayer::kTypeSearchNone;
			SearchMusicLayer::stringToSearch = "";
			return ret;
		}

		auto retModified = cocos2d::CCArray::createWithCapacity(ret->count());

		for (size_t i = 0; i < ret->count(); i++)
		{
			auto musicObj = reinterpret_cast<SongInfoObject*>(ret->objectAtIndex(i));

			int score;

			if (SearchMusicLayer::typeSearch == kTypeSearchMusicLayer::kTypeSearchSongName) {

				if (!musicObj->m_songName.empty() &&
					fts::fuzzy_match(SearchMusicLayer::stringToSearch.c_str(), musicObj->m_songName.c_str(), score)) {
					retModified->addObject(musicObj);
				}
			}
			else {
				if (!musicObj->m_artistName.empty() &&
					fts::fuzzy_match(SearchMusicLayer::stringToSearch.c_str(), musicObj->m_artistName.c_str(), score)) {
					retModified->addObject(musicObj);
				}
			}
		}


		return retModified;
	}
};

class $modify(GJDropDownLayer) {
	void hideLayer(bool isNose) {
		if (typeinfo_cast<GJSongBrowser*>(this)) {
			SearchMusicLayer::typeSearch = kTypeSearchMusicLayer::kTypeSearchNone;
			SearchMusicLayer::stringToSearch = "";
		}
		GJDropDownLayer::hideLayer(isNose);
	}
};

class $modify(CCNode) {
	void addChild(CCNode * child, int zOrder, int tag) {
		if (std::string(Utils::getNameObj(child)) == "GJSongBrowser") {
			zOrder = Utils::shareDirectorA()->getRunningScene()->getHighestChildZ();
		}
		CCNode::addChild(child, zOrder, tag);
	}
};

class $modify(CCScene) {
	static CCScene* create() {
		auto ret = CCScene::create();

		std::cout << std::hex << offsetof(CustomSongWidget, m_songDelegate) << std::endl;

		return ret;
	}

};