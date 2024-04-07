#include <Geode/Geode.hpp>
#include "Utils.hpp"
#include "fts_fuzzy_match.hpp"

using namespace geode::prelude;

enum kTypeSearchMusicLayer {
	kTypeSearchNone = 0,
	kTypeSearchSongName = 1,
	kTypeSearchArtistName = 2
};

class SearchMusicLayer : public Popup<GJSongBrowser*>, public cocos2d::CCTextFieldDelegate, public TextInputDelegate {
public:

	static SearchMusicLayer* create(GJSongBrowser* parent);
	bool setup(GJSongBrowser* parent) override;
	virtual void keyBackClicked();
	void keyDown(cocos2d::enumKeyCodes key);
	void onClose(cocos2d::CCObject* pSender);
	void onSearchMusic(cocos2d::CCObject* pSender);
	void onSearchArtist(cocos2d::CCObject* pSender);
	virtual void textChanged(CCTextInputNode* input);
	void updateParentAContent();
	void onSettings(cocos2d::CCObject* pSender);
	void onDeleteTextSearch(cocos2d::CCObject* pSender);

	static kTypeSearchMusicLayer typeSearch;
	static std::string stringToSearch;
	cocos2d::extension::CCScale9Sprite* m_pBG = nullptr;
	cocos2d::CCMenu* m_pMenuButtons = nullptr;
	cocos2d::CCLabelBMFont* m_TitleLabel = nullptr;
	cocos2d::CCLabelBMFont* m_pDescriptionLabel = nullptr;
	cocos2d::extension::CCScale9Sprite* m_pBGTextInput = nullptr;
	cocos2d::extension::CCScale9Sprite* m_pBGTextInput_2 = nullptr;
	CCTextInputNode* m_pTextInput = nullptr;
	CCMenuItemSpriteExtra* m_pSearchMusicName = nullptr;
	CCMenuItemSpriteExtra* m_pSearchArtistName = nullptr;
	GJSongBrowser* m_pParentA = nullptr;

};