#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>
#include <Geode/ui/TextInput.hpp>

using namespace geode::prelude;

// code snippet taken from https://github.com/geode-sdk/textureldr/blob/main/src/PackSelectLayer.hpp
class FLCustomizerLayer : public CCLayer {
protected:
	bool init() override;
	TextInput* m_titleInput;
	TextInput* m_contentInput;
public:
	static FLCustomizerLayer* create();
	static FLCustomizerLayer* scene();

	void onBack(CCObject*);
	void onButton(CCObject*);
};

// from https://github.com/geode-sdk/textureldr/blob/main/src/PackSelectLayer.cpp

bool FLCustomizerLayer::init() {
	if (!CCLayer::init()) {
		return false;
	}

	this->setID("flhacklayer");
	this->setKeypadEnabled(true);

	auto bg = createLayerBG();
	bg->setID("background");
	this->addChild(bg);

	auto winSize = CCDirector::get()->getWinSize();

	auto menu = CCMenu::create();
	menu->setID("menu");
	menu->setZOrder(10);

	auto backBtn = CCMenuItemSpriteExtra::create(
		CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"),
		this,
		menu_selector(FLCustomizerLayer::onBack)
	);

	backBtn->setPosition(-winSize.width / 2 + 25.f, winSize.height / 2 - 25.f);
	backBtn->setID("back-button");
	menu->addChild(backBtn);

	auto titleLabel = CCLabelBMFont::create("Title", "bigFont.fnt");
	titleLabel->setID("fltitlelabel");
	titleLabel->setPositionX(174);
	titleLabel->setPositionY(280);

	this->m_titleInput = TextInput::create(
		200,
		"Title"
	);
	this->m_titleInput->setID("fltitleinput");
	this->m_titleInput->setCommonFilter(CommonFilter::Any);
	this->m_titleInput->setPositionX(175);
	this->m_titleInput->setPositionY(239);

	auto contentLabel = CCLabelBMFont::create("Content", "bigFont.fnt");
	contentLabel->setID("flcontentlabel");
	contentLabel->setPositionX(174);
	contentLabel->setPositionY(199);

	this->m_contentInput = TextInput::create(
		200,
		"Content"
	);
	this->m_contentInput->setID("flcontentinput");
	this->m_contentInput->setCommonFilter(CommonFilter::Any);
	this->m_contentInput->setPositionX(173);
	this->m_contentInput->setPositionY(160);

	auto btn = CCMenuItemSpriteExtra::create(
		ButtonSprite::create("Confirm"),
		this,
		menu_selector(FLCustomizerLayer::onButton)
	);
	btn->setID("flconfirm");
	btn->setPositionX(-108);
	btn->setPositionY(-49);

	this->addChild(titleLabel);
	this->addChild(this->m_titleInput);

	this->addChild(contentLabel);
	this->addChild(this->m_contentInput);
	menu->addChild(btn);
	this->addChild(menu);

	return true;
}

void FLCustomizerLayer::onBack(CCObject*) {
	CCDirector::get()->replaceScene(CCTransitionFade::create(0.5, MenuLayer::scene(false)));
}

void FLCustomizerLayer::onButton(CCObject*) {
	if (this->m_titleInput || this->m_contentInput) {
		if (this->m_titleInput->getString().empty() || this->m_contentInput->getString().empty()) {
			FLAlertLayer::create(
				"Error!",
				"<cr>You need to enter a title and some content.</c>",
				"OK"
			)->show();
		}
		else {
			FLAlertLayer::create(
				this->m_titleInput->getString().c_str(),
				this->m_contentInput->getString(),
				"OK"
			)->show();
		}
	}
}

FLCustomizerLayer* FLCustomizerLayer::create() {
	auto ret = new FLCustomizerLayer;
	if (ret->init()) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

FLCustomizerLayer* FLCustomizerLayer::scene() {
	auto layer = FLCustomizerLayer::create();
	switchToScene(layer);
	return layer;
}

class $modify(Menu, MenuLayer) {
	bool init() {
		if (!MenuLayer::init()) {
			return false;
		}

		auto btn = CCMenuItemSpriteExtra::create(
			CircleButtonSprite::createWithSprite("popup.png"_spr),
			this,
			menu_selector(Menu::onMenuButton)
		);

		auto menu = this->getChildByID("bottom-menu");
		menu->addChild(btn);
		btn->setID("flhackbtn");

		menu->updateLayout();

		return true;
	}

	void onMenuButton(CCObject*) {
		FLCustomizerLayer::scene();
	}
};