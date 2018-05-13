#pragma once

#include "json.h"
#include "date_time.h"

//
// This is basically a 1 to 1 encoding of the CMS data
//

struct CMS_Achievement : public Json::ISerializeable
{
	int AchievementId;
	int AchievementScore;
	int Difficulty;
	std::string StartCondition;
	std::string StatisticsTrace;
	int ReachedCount;
	std::string TitleTextKey;
	std::string DescTextKey;
	std::string ConditionTextKey;

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(AchievementId);
		SERIALIZE_JSON(AchievementScore);
		SERIALIZE_JSON(Difficulty);
		SERIALIZE_JSON(StartCondition);
		SERIALIZE_JSON(StatisticsTrace);
		SERIALIZE_JSON(ReachedCount);
		SERIALIZE_JSON(TitleTextKey);
		SERIALIZE_JSON(DescTextKey);
		SERIALIZE_JSON(ConditionTextKey);
	}
};
struct CMS_AchievementReward : public Json::ISerializeable
{
	int Score;
	std::string Reward1;
	int Reward1Amount;
	std::string Reward2;
	int Reward2Amount;
	std::string Reward3;
	int Reward3Amount;

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(Score);
		SERIALIZE_JSON(Reward1);
		SERIALIZE_JSON(Reward1Amount);
		SERIALIZE_JSON(Reward2);
		SERIALIZE_JSON(Reward2Amount);
		SERIALIZE_JSON(Reward3);
		SERIALIZE_JSON(Reward3Amount);
	}
};
struct CMS_AttendanceReward : public Json::ISerializeable
{
	int AttendanceID;
	int DayNumber;
	std::string ItemCode;
	int ItemCount;
	int ApplyType;

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(AttendanceID);
		SERIALIZE_JSON(DayNumber);
		SERIALIZE_JSON(ItemCode);
		SERIALIZE_JSON(ItemCount);
		SERIALIZE_JSON(ApplyType);
	}
};
struct CMS_ConfigData : public Json::ISerializeable
{
	int ConfigType;
	int Step;
	std::string ItemCode;
	int Value;

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(ConfigType);
		SERIALIZE_JSON(Step);
		SERIALIZE_JSON(ItemCode);
		SERIALIZE_JSON(Value);
	}
};
struct CMS_Designation : public Json::ISerializeable
{
	std::string ItemCode;
	int EffectType;
	int EffectCondition1;
	int EffectCondition2;
	float EffectAmount;
	std::string AcquireConditionTextKey;
	std::string EffectTextKey;
	int SortOrder;

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(ItemCode);
		SERIALIZE_JSON(EffectType);
		SERIALIZE_JSON(EffectCondition1);
		SERIALIZE_JSON(EffectCondition2);
		SERIALIZE_JSON(EffectAmount);
		SERIALIZE_JSON(AcquireConditionTextKey);
		SERIALIZE_JSON(EffectTextKey);
		SERIALIZE_JSON(SortOrder);
	}
};
struct CMS_Guide : public Json::ISerializeable
{
	int GuideID;
	int Step;
	int ShowCharacter;
	int ShowComment;
	std::string CommentText;
	int CommentPosType;
	int CommentPosY;
	int ShowTarget;
	int ShowFocus;
	int NextType;
	float NextValue;
	int TargetID1;
	int TargetID2;
	int TargetID3;

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(GuideID);
		SERIALIZE_JSON(Step);
		SERIALIZE_JSON(ShowCharacter);
		SERIALIZE_JSON(ShowComment);
		SERIALIZE_JSON(CommentText);
		SERIALIZE_JSON(CommentPosType);
		SERIALIZE_JSON(CommentPosY);
		SERIALIZE_JSON(ShowTarget);
		SERIALIZE_JSON(ShowFocus);
		SERIALIZE_JSON(NextType);
		SERIALIZE_JSON(NextValue);
		SERIALIZE_JSON(TargetID1);
		SERIALIZE_JSON(TargetID2);
		SERIALIZE_JSON(TargetID3);
	}
};
struct CMS_Item : public Json::ISerializeable
{
	enum ItemTypeCodes
	{
		Item			= 1,
		// ???
		Gold			= 3,
		StartCube		= 4,
		Skin			= 5,
		Supporter		= 6,
		Designation		= 7,
		DailyPresent	= 8,
		// ???
		// ???
		StandardSong =	 11,
	};

	std::string ItemCode;
	int ItemType;
	std::string ItemName;
	std::string Desc;
	std::string ItemImageFile;

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(ItemCode);
		SERIALIZE_JSON(ItemType);
		SERIALIZE_JSON(ItemName);
		SERIALIZE_JSON(Desc);
		SERIALIZE_JSON(ItemImageFile);
	}
};
struct CMS_KeynumLib : public Json::ISerializeable
{
	std::string LibName;
	int SortOrder;
	std::string keyNum;
	int startPosIndex;
	int enemyStartPos;
	double duration;
	std::string Column;

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(LibName);
		SERIALIZE_JSON(SortOrder);
		SERIALIZE_JSON(keyNum);
		SERIALIZE_JSON(startPosIndex);
		SERIALIZE_JSON(enemyStartPos);
		SERIALIZE_JSON(duration);
		SERIALIZE_JSON(Column);
	}
};
struct CMS_KeynumLib_Tutor : public Json::ISerializeable
{
	std::string keyNum;
	int startPosIndex;
	int endPosIndex;
	int shotEventIndex;
	int enemyStartPos;
	std::string durationIsStartSec;
	double duration;

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(keyNum);
		SERIALIZE_JSON(startPosIndex);
		SERIALIZE_JSON(endPosIndex);
		SERIALIZE_JSON(shotEventIndex);
		SERIALIZE_JSON(enemyStartPos);
		SERIALIZE_JSON(durationIsStartSec);
		SERIALIZE_JSON(duration);
	}
};
struct CMS_LaneNoteType : public Json::ISerializeable
{
	int laneNum;
	std::string noteType;
	std::string weaponType;

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(laneNum);
		SERIALIZE_JSON(noteType);
		SERIALIZE_JSON(weaponType);
	}
};
struct CMS_MasterValue : public Json::ISerializeable
{
	std::string KeyCode;
	std::string Value;
	int IntValue;

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(KeyCode);
		SERIALIZE_JSON(Value);
		SERIALIZE_JSON(IntValue);
	}
};
struct CMS_Mode_Chaotic : public Json::ISerializeable
{
	int MusicNo;
	int PatternNo;
	int Difficulty;
	std::string PatternName;
	std::string ThumbImageTag;
	std::string ThumbImageFile;
	std::string MusicFile;
	int MusicStartSec;
	int MusicEndSec;
	std::string RewardDesignation;

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(MusicNo);
		SERIALIZE_JSON(PatternNo);
		SERIALIZE_JSON(Difficulty);
		SERIALIZE_JSON(PatternName);
		SERIALIZE_JSON(ThumbImageTag);
		SERIALIZE_JSON(ThumbImageFile);
		SERIALIZE_JSON(MusicFile);
		SERIALIZE_JSON(MusicStartSec);
		SERIALIZE_JSON(MusicEndSec);
		SERIALIZE_JSON(RewardDesignation);
	}
};
struct CMS_Mode_Chaotic_Group : public Json::ISerializeable
{
	int GroupId;
	std::string Name;
	int SortOrder;

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(GroupId);
		SERIALIZE_JSON(Name);
		SERIALIZE_JSON(SortOrder);
	}
};
struct CMS_Mode_Chaotic_GroupList : public Json::ISerializeable
{
	int GroupId;
	int MusicNo;
	int PatternNo;
	int SortOrder;

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(GroupId);
		SERIALIZE_JSON(MusicNo);
		SERIALIZE_JSON(PatternNo);
		SERIALIZE_JSON(SortOrder);
	}
};
struct CMS_Mode_Chaotic_Mission : public Json::ISerializeable
{
	int MissionId;
	int MusicNo;
	int PatternNo;
	int Mission1Type;
	int Mission1Value;
	int Mission1Comparison;
	int Mission2Type;
	int Mission2Value;
	int Mission2Comparison;
	int Mission3Type;
	int Mission3Value;
	int Mission3Comparison;
	bool ShowGauge;
	bool IsAcumulative;
	std::string DescKey;

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(MissionId);
		SERIALIZE_JSON(MusicNo);
		SERIALIZE_JSON(PatternNo);
		SERIALIZE_JSON(Mission1Type);
		SERIALIZE_JSON(Mission1Value);
		SERIALIZE_JSON(Mission1Comparison);
		SERIALIZE_JSON(Mission2Type);
		SERIALIZE_JSON(Mission2Value);
		SERIALIZE_JSON(Mission2Comparison);
		SERIALIZE_JSON(Mission3Type);
		SERIALIZE_JSON(Mission3Value);
		SERIALIZE_JSON(Mission3Comparison);
		SERIALIZE_JSON(ShowGauge);
		SERIALIZE_JSON(IsAcumulative);
		SERIALIZE_JSON(DescKey);
	}
};
struct CMS_Mode_Episode : public Json::ISerializeable
{
	int EpisodeID;
	std::string ScenarioID;
	int CommOrder;
	std::string CharCode;
	int StageNo;
	std::string Title;
	int NeedGold;
	std::string NeedItemCode1;
	int NeedItemCount1;
	std::string NeedItemCode2;
	int NeedItemCount2;
	std::string RewardItemCode;
	int RewardItemCount;
	int RewardMusicNo;
	int MusicNo1;
	int PatternNo1;
	int MusicNo2;
	int PatternNo2;
	int MusicNo3;
	int PatternNo3;
	int UnlockType;
	int UnlockValue;

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(EpisodeID);
		SERIALIZE_JSON(ScenarioID);
		SERIALIZE_JSON(CommOrder);
		SERIALIZE_JSON(CharCode);
		SERIALIZE_JSON(StageNo);
		SERIALIZE_JSON(Title);
		SERIALIZE_JSON(NeedGold);
		SERIALIZE_JSON(NeedItemCode1);
		SERIALIZE_JSON(NeedItemCount1);
		SERIALIZE_JSON(NeedItemCode2);
		SERIALIZE_JSON(NeedItemCount2);
		SERIALIZE_JSON(RewardItemCode);
		SERIALIZE_JSON(RewardItemCount);
		SERIALIZE_JSON(RewardMusicNo);
		SERIALIZE_JSON(MusicNo1);
		SERIALIZE_JSON(PatternNo1);
		SERIALIZE_JSON(MusicNo2);
		SERIALIZE_JSON(PatternNo2);
		SERIALIZE_JSON(MusicNo3);
		SERIALIZE_JSON(PatternNo3);
		SERIALIZE_JSON(UnlockType);
		SERIALIZE_JSON(UnlockValue);
	}
};
struct CMS_Mode_Episode_Char : public Json::ISerializeable
{
	std::string CharCode;
	int OrderIndex;
	std::string CharName;
	std::string StandingImgName;
	int StandingImgPosX;
	int StandingImgPosY;
	int StandingImgWidth;
	int StandingImgHeight;
	int StandingSerifPosX;
	int StandingSerifPosY;
	std::string ThumbImgName;
	std::string DefaultSerifText;
	int UnlockType;
	int UnlockValue;

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(CharCode);
		SERIALIZE_JSON(OrderIndex);
		SERIALIZE_JSON(CharName);
		SERIALIZE_JSON(StandingImgName);
		SERIALIZE_JSON(StandingImgPosX);
		SERIALIZE_JSON(StandingImgPosY);
		SERIALIZE_JSON(StandingImgWidth);
		SERIALIZE_JSON(StandingImgHeight);
		SERIALIZE_JSON(StandingSerifPosX);
		SERIALIZE_JSON(StandingSerifPosY);
		SERIALIZE_JSON(ThumbImgName);
		SERIALIZE_JSON(DefaultSerifText);
		SERIALIZE_JSON(UnlockType);
		SERIALIZE_JSON(UnlockValue);
	}
};
struct CMS_Mode_Standard : public Json::ISerializeable
{
	int MusicNo;
	int StageType;
	std::string ThumbImgTag;
	std::string ThumbImgFile;
	std::string CoverArtImgFile;
	std::string MusicFileName;
	int MusicStartSec;
	int MusicEndSec;
	int SongSortOrder;
	int IsNowSale;
	int NationNo;

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(MusicNo);
		SERIALIZE_JSON(StageType);
		SERIALIZE_JSON(ThumbImgTag);
		SERIALIZE_JSON(ThumbImgFile);
		SERIALIZE_JSON(CoverArtImgFile);
		SERIALIZE_JSON(MusicFileName);
		SERIALIZE_JSON(MusicStartSec);
		SERIALIZE_JSON(MusicEndSec);
		SERIALIZE_JSON(SongSortOrder);
		SERIALIZE_JSON(IsNowSale);
		SERIALIZE_JSON(NationNo);
	}
};
struct CMS_Mode_Standard_Group : public Json::ISerializeable
{
	int GroupNo;
	std::string GroupName;
	int SortOrder;
	
	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(GroupNo);
		SERIALIZE_JSON(GroupName);
		SERIALIZE_JSON(SortOrder);
	}
};
struct CMS_Mode_Standard_GroupList : public Json::ISerializeable
{
	int GroupNo;
	int MusicNo;
	int SortOrder;

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(GroupNo);
		SERIALIZE_JSON(MusicNo);
		SERIALIZE_JSON(SortOrder);
	}
};
struct CMS_Mode_Story : public Json::ISerializeable
{
	int StoryNo;
	int MusicNo;
	std::string StageName;
	std::string CoverArtImgName;
	std::string ThumbImgName;
	std::string UtageIntroName;
	std::string UtageOutroName;
	std::string UtageFailName;
	std::string UnlockStatusText;
	int GameBgType;
	int UnlockType;
	int UnlockValue;
	std::string SynopsisText;
	std::string UnlockItem;

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(StoryNo);
		SERIALIZE_JSON(MusicNo);
		SERIALIZE_JSON(StageName);
		SERIALIZE_JSON(CoverArtImgName);
		SERIALIZE_JSON(ThumbImgName);
		SERIALIZE_JSON(UtageIntroName);
		SERIALIZE_JSON(UtageOutroName);
		SERIALIZE_JSON(UtageFailName);
		SERIALIZE_JSON(UnlockStatusText);
		SERIALIZE_JSON(GameBgType);
		SERIALIZE_JSON(UnlockType);
		SERIALIZE_JSON(UnlockValue);
		SERIALIZE_JSON(SynopsisText);
		SERIALIZE_JSON(UnlockItem);
	}
};
struct CMS_Music : public Json::ISerializeable
{
	int MusicNo;
	std::string SongName;
	std::string ComposerName;
	std::string MusicName;
	std::string IllustratorName;
	std::string CoverArtImage;
	int ResultCoverArt_Width;
	int ResultCoverArt_Height;
	int ResultCoverArt_PosX;
	int ResultCoverArt_PosY;
	int ResultCoverArt_RotZ;

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(MusicNo);
		SERIALIZE_JSON(SongName);
		SERIALIZE_JSON(ComposerName);
		SERIALIZE_JSON(MusicName);
		SERIALIZE_JSON(IllustratorName);
		SERIALIZE_JSON(CoverArtImage);
		SERIALIZE_JSON(ResultCoverArt_Width);
		SERIALIZE_JSON(ResultCoverArt_Height);
		SERIALIZE_JSON(ResultCoverArt_PosX);
		SERIALIZE_JSON(ResultCoverArt_PosY);
		SERIALIZE_JSON(ResultCoverArt_RotZ);
	}
};
struct CMS_Music_Pattern : public Json::ISerializeable
{
	int MusicNo;
	int PatternNo;
	std::string Difficulty;
	int Level;
	std::string BmsSheetFileName;
	std::string CsvSheetFileName;
	std::string EventListFileName;
	std::string EnemyFileName;
	float MusicEndTime;
	bool UseThreeBMS;
	std::string GameEventJsonFileName;

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(MusicNo);
		SERIALIZE_JSON(PatternNo);
		SERIALIZE_JSON(Difficulty);
		SERIALIZE_JSON(Level);
		SERIALIZE_JSON(BmsSheetFileName);
		SERIALIZE_JSON(CsvSheetFileName);
		SERIALIZE_JSON(EventListFileName);
		SERIALIZE_JSON(EnemyFileName);
		SERIALIZE_JSON(MusicEndTime);
		SERIALIZE_JSON(UseThreeBMS);
		SERIALIZE_JSON(GameEventJsonFileName);
	}
};
struct CMS_Notice : public Json::ISerializeable
{
	int NoticeID;
	int NoticeType;
	std::string Country;
	std::string Title;
	std::string ImgFilePath;
	int ActionType;
	std::string ActionURL;
	int ExpireType;
	DateTime StartDate;
	DateTime EndDate;

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(NoticeID);
		SERIALIZE_JSON(NoticeType);
		SERIALIZE_JSON(Country);
		SERIALIZE_JSON(Title);
		SERIALIZE_JSON(ImgFilePath);
		SERIALIZE_JSON(ActionType);
		SERIALIZE_JSON(ActionURL);
		SERIALIZE_JSON(ExpireType);
		SERIALIZE_JSON(StartDate);
		SERIALIZE_JSON(EndDate);
	}
};
struct CMS_Quest : public Json::ISerializeable
{
	int QuestID;
	int QuestType;
	int QuestCategory;
	bool IsPopup;
	std::string StartCondition;
	std::string StatisticsTrace;
	std::string MoveSceneName;
	int ReachedCount;
	std::string Reward1ItemCode;
	int Reward1ItemCount;
	std::string Reward2ItemCode;
	int Reward2ItemCount;
	std::string TitleTextKey;
	std::string ConditionTextKey;

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(QuestID);
		SERIALIZE_JSON(QuestType);
		SERIALIZE_JSON(QuestCategory);
		SERIALIZE_JSON(IsPopup);
		SERIALIZE_JSON(StartCondition);
		SERIALIZE_JSON(StatisticsTrace);
		SERIALIZE_JSON(MoveSceneName);
		SERIALIZE_JSON(ReachedCount);
		SERIALIZE_JSON(Reward1ItemCode);
		SERIALIZE_JSON(Reward1ItemCount);
		SERIALIZE_JSON(Reward2ItemCode);
		SERIALIZE_JSON(Reward2ItemCount);
		SERIALIZE_JSON(TitleTextKey);
		SERIALIZE_JSON(ConditionTextKey);
	}
};
struct CMS_Shop_BuyGold : public Json::ISerializeable
{
	int Step;
	int MusicGold;
	int SupportGold;

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(Step);
		SERIALIZE_JSON(MusicGold);
		SERIALIZE_JSON(SupportGold);
	}
};
struct CMS_Shop_Package : public Json::ISerializeable
{
	std::string PackageCode;
	int PackageType;
	std::string PackageText_Eng;
	std::string Title;
	std::string SeasonText;
	std::string AssetBundleTag;
	std::string ThumbImg;
	std::string BannerImg;
	std::string StandardImg;
	int PriceType;
	int Price_Origin;
	int Price_Now;
	int SortOrder;
	std::string ShopDescription;
	int MusicNo;
	std::string MusicFileName;
	int MusicStartSec;

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(PackageCode);
		SERIALIZE_JSON(PackageType);
		SERIALIZE_JSON(PackageText_Eng);
		SERIALIZE_JSON(Title);
		SERIALIZE_JSON(SeasonText);
		SERIALIZE_JSON(AssetBundleTag);
		SERIALIZE_JSON(ThumbImg);
		SERIALIZE_JSON(BannerImg);
		SERIALIZE_JSON(StandardImg);
		SERIALIZE_JSON(PriceType);
		SERIALIZE_JSON(Price_Origin);
		SERIALIZE_JSON(Price_Now);
		SERIALIZE_JSON(SortOrder);
		SERIALIZE_JSON(ShopDescription);
		SERIALIZE_JSON(MusicNo);
		SERIALIZE_JSON(MusicFileName);
		SERIALIZE_JSON(MusicStartSec);
	}
};
struct CMS_Shop_PackageDetail : public Json::ISerializeable
{
	std::string PackageCode;
	int PackageCodeRow;
	int IsArcade;
	int MusicNo;
	int IsSkin;
	std::string SkinCode;
	int IsCommunity;
	std::string CommunityCode;
	std::string CommunityCharaCode;
	int GoodsType;
	std::string GoodsCode;
	int GoodsCount;

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(PackageCode);
		SERIALIZE_JSON(PackageCodeRow);
		SERIALIZE_JSON(IsArcade);
		SERIALIZE_JSON(MusicNo);
		SERIALIZE_JSON(IsSkin);
		SERIALIZE_JSON(SkinCode);
		SERIALIZE_JSON(IsCommunity);
		SERIALIZE_JSON(CommunityCode);
		SERIALIZE_JSON(CommunityCharaCode);
		SERIALIZE_JSON(GoodsType);
		SERIALIZE_JSON(GoodsCode);
		SERIALIZE_JSON(GoodsCount);
	}
};
struct CMS_Support : public Json::ISerializeable
{
	std::string SupportID;
	int Category;
	int Grade;
	int SummonTier;
	std::string Name;
	std::string Description;
	std::string ImageName;

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(SupportID);
		SERIALIZE_JSON(Category);
		SERIALIZE_JSON(Grade);
		SERIALIZE_JSON(SummonTier);
		SERIALIZE_JSON(Name);
		SERIALIZE_JSON(Description);
		SERIALIZE_JSON(ImageName);
	}
};
struct CMS_Support_Grade : public Json::ISerializeable
{
	int Grade;
	int Level;
	float CoolTime;
	int NeedGold;
	std::string NeedItemCode1;
	int NeedItemCount1;
	std::string NeedItemCode2;
	int NeedItemCount2;
	int NeedCardCount;
	int CardExp;
	int NeedGemExp;
	int GemPower;

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(Grade);
		SERIALIZE_JSON(Level);
		SERIALIZE_JSON(CoolTime);
		SERIALIZE_JSON(NeedGold);
		SERIALIZE_JSON(NeedItemCode1);
		SERIALIZE_JSON(NeedItemCount1);
		SERIALIZE_JSON(NeedItemCode2);
		SERIALIZE_JSON(NeedItemCount2);
		SERIALIZE_JSON(NeedCardCount);
		SERIALIZE_JSON(CardExp);
		SERIALIZE_JSON(NeedGemExp);
		SERIALIZE_JSON(GemPower);
	}
};
struct CMS_Support_StageSet : public Json::ISerializeable
{
	int MusicNo;
	std::string Left;
	std::string LeftMid;
	std::string RightMid;
	std::string Right;

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(MusicNo);
		SERIALIZE_JSON(Left);
		SERIALIZE_JSON(LeftMid);
		SERIALIZE_JSON(RightMid);
		SERIALIZE_JSON(Right);
	}
};
struct CMS_Support_Value : public Json::ISerializeable
{
	std::string SupportID;
	int Level;
	int Value;
	int NextCardRequire;

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(SupportID);
		SERIALIZE_JSON(Level);
		SERIALIZE_JSON(Value);
		SERIALIZE_JSON(NextCardRequire);
	}
};
struct CMS_Table_Update : public Json::ISerializeable
{
	std::string TableName;
	DateTime UpdateDate;

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(TableName);
		SERIALIZE_JSON(UpdateDate);
	}
};
struct CMS_TEXT_String : public Json::ISerializeable
{
	// TODO - can save a lot of space by excluding languages we don't use/care about for the specific client
	std::string TextKey;
	std::string Text_KR;
	std::string Text_EN;
	std::string Text_CN;
	std::string Text_TW;
	std::string Text_JP;

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(TextKey);
		SERIALIZE_JSON(Text_KR);
		SERIALIZE_JSON(Text_EN);
		SERIALIZE_JSON(Text_CN);
		SERIALIZE_JSON(Text_TW);
		SERIALIZE_JSON(Text_JP);
	}
};
struct CMS_UI_String : public Json::ISerializeable
{
	// TODO - can save a lot of space by excluding languages we don't use/care about for the specific client
	std::string UIStrKey;
	std::string Text_KR;
	int Size_KR;
	int FontNo_KR;
	std::string Text_EN;
	int Size_EN;
	int FontNo_EN;
	std::string Text_CN;
	int Size_CN;
	int FontNo_CN;
	std::string Text_TW;
	int Size_TW;
	int FontNo_TW;
	std::string Text_JP;
	int Size_JP;
	int FontNo_JP;

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(UIStrKey);
		SERIALIZE_JSON(Text_KR);
		SERIALIZE_JSON(Size_KR);
		SERIALIZE_JSON(FontNo_KR);
		SERIALIZE_JSON(Text_EN);
		SERIALIZE_JSON(Size_EN);
		SERIALIZE_JSON(FontNo_EN);
		SERIALIZE_JSON(Text_CN);
		SERIALIZE_JSON(Size_CN);
		SERIALIZE_JSON(FontNo_CN);
		SERIALIZE_JSON(Text_TW);
		SERIALIZE_JSON(Size_TW);
		SERIALIZE_JSON(FontNo_TW);
		SERIALIZE_JSON(Text_JP);
		SERIALIZE_JSON(Size_JP);
		SERIALIZE_JSON(FontNo_JP);
	}
};
struct CMS_Unlock_Condition : public Json::ISerializeable
{
	int TargetID;
	int TargetValue;
	int UnlockConditionID;
	int UnlockValue;
	int UnlockQuantity;

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(TargetID);
		SERIALIZE_JSON(TargetValue);
		SERIALIZE_JSON(UnlockConditionID);
		SERIALIZE_JSON(UnlockValue);
		SERIALIZE_JSON(UnlockQuantity);
	}
};
struct CMS_UserLevel : public Json::ISerializeable
{
	int Level;
	int Grade;
	int GradeLevel;
	int NeedExp;
	int TotalNeedExp;
	std::string RewardItemCode;
	int RewardItemCount;

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(Level);
		SERIALIZE_JSON(Grade);
		SERIALIZE_JSON(GradeLevel);
		SERIALIZE_JSON(NeedExp);
		SERIALIZE_JSON(TotalNeedExp);
		SERIALIZE_JSON(RewardItemCode);
		SERIALIZE_JSON(RewardItemCount);
	}
};
struct CMS_WeeklyRankingReward : public Json::ISerializeable
{
	int Tier;
	std::string ItemCode;
	int ItemCount;

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(Tier);
		SERIALIZE_JSON(ItemCode);
		SERIALIZE_JSON(ItemCount);
	}
};
struct CMS_WeeklyRankingTier : public Json::ISerializeable
{
	int Tier;
	int TierType;
	int RankingInt;
	float RankingPercentage;
	int RankingScore;
	std::string TierTextKey;

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(Tier);
		SERIALIZE_JSON(TierType);
		SERIALIZE_JSON(RankingInt);
		SERIALIZE_JSON(RankingPercentage);
		SERIALIZE_JSON(RankingScore);
		SERIALIZE_JSON(TierTextKey);
	}
};
struct CMS_LoadingTip : public Json::ISerializeable
{
	int TipID;
	int StartScene;
	int EndScene;
	std::string Tip;

	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(TipID);
		SERIALIZE_JSON(StartScene);
		SERIALIZE_JSON(EndScene);
		SERIALIZE_JSON(Tip);
	}
};

struct CMSData : public Json::ISerializeable
{
	bool isUpdate = false;
	std::vector<CMS_Achievement> cms_Achievement;
	std::vector<CMS_AchievementReward> cms_AchievementReward;
	std::vector<CMS_AttendanceReward> cms_AttendanceReward;
	std::vector<CMS_ConfigData> cms_ConfigData;
	std::vector<CMS_Designation> cms_Designation;
	std::vector<CMS_Guide> cms_Guide;
	std::vector<CMS_Item> cms_Item;
	std::vector<CMS_KeynumLib> cms_KeynumLib;
	std::vector<CMS_KeynumLib_Tutor> cms_KeynumLib_Tutor;
	std::vector<CMS_LaneNoteType> cms_LaneNoteType;
	std::vector<CMS_MasterValue> cms_MasterValue;
	std::vector<CMS_Mode_Chaotic> cms_Mode_Chaotic;
	std::vector<CMS_Mode_Chaotic_Group> cms_Mode_Chaotic_Group;
	std::vector<CMS_Mode_Chaotic_GroupList> cms_Mode_Chaotic_GroupList;
	std::vector<CMS_Mode_Chaotic_Mission> cms_Mode_Chaotic_Mission;
	std::vector<CMS_Mode_Episode> cms_Mode_Episode;
	std::vector<CMS_Mode_Episode_Char> cms_Mode_Episode_Char;
	std::vector<CMS_Mode_Standard> cms_Mode_Standard;
	std::vector<CMS_Mode_Standard_Group> cms_Mode_Standard_Group;
	std::vector<CMS_Mode_Standard_GroupList> cms_Mode_Standard_GroupList;
	std::vector<CMS_Mode_Story> cms_Mode_Story;
	std::vector<CMS_Music> cms_Music;
	std::vector<CMS_Music_Pattern> cms_Music_Pattern;
	std::vector<CMS_Notice> cms_Notice;
	std::vector<CMS_Quest> cms_Quest;
	std::vector<CMS_Shop_BuyGold> cms_Shop_BuyGold;
	std::vector<CMS_Shop_Package> cms_Shop_Package;
	std::vector<CMS_Shop_PackageDetail> cms_Shop_PackageDetail;
	std::vector<CMS_Support> cms_Support;
	std::vector<CMS_Support_Grade> cms_Support_Grade;
	std::vector<CMS_Support_StageSet> cms_Support_StageSet;
	std::vector<CMS_Support_Value> cms_Support_Value;
	std::vector<CMS_Table_Update> cms_Table_Update;
	std::vector<CMS_TEXT_String> cms_TEXT_String;
	std::vector<CMS_UI_String> cms_UI_String;
	std::vector<CMS_Unlock_Condition> cms_Unlock_Condition;
	std::vector<CMS_UserLevel> cms_UserLevel;
	std::vector<CMS_WeeklyRankingReward> cms_WeeklyRankingReward;
	std::vector<CMS_WeeklyRankingTier> cms_WeeklyRankingTier;
	std::vector<CMS_LoadingTip> cms_LoadingTip;


	void Serialize(Json::Serializer& serializer) override
	{
		SERIALIZE_JSON(isUpdate);
		SERIALIZE_JSON(cms_Achievement);
		SERIALIZE_JSON(cms_AchievementReward);
		SERIALIZE_JSON(cms_AttendanceReward);
		SERIALIZE_JSON(cms_ConfigData);
		SERIALIZE_JSON(cms_Designation);
		SERIALIZE_JSON(cms_Guide);
		SERIALIZE_JSON(cms_Item);
		SERIALIZE_JSON(cms_KeynumLib);
		SERIALIZE_JSON(cms_KeynumLib_Tutor);
		SERIALIZE_JSON(cms_LaneNoteType);
		SERIALIZE_JSON(cms_MasterValue);
		SERIALIZE_JSON(cms_Mode_Chaotic);
		SERIALIZE_JSON(cms_Mode_Chaotic_Group);
		SERIALIZE_JSON(cms_Mode_Chaotic_GroupList);
		SERIALIZE_JSON(cms_Mode_Chaotic_Mission);
		SERIALIZE_JSON(cms_Mode_Episode);
		SERIALIZE_JSON(cms_Mode_Episode_Char);
		SERIALIZE_JSON(cms_Mode_Standard);
		SERIALIZE_JSON(cms_Mode_Standard_Group);
		SERIALIZE_JSON(cms_Mode_Standard_GroupList);
		SERIALIZE_JSON(cms_Mode_Story);
		SERIALIZE_JSON(cms_Music);
		SERIALIZE_JSON(cms_Music_Pattern);
		SERIALIZE_JSON(cms_Notice);
		SERIALIZE_JSON(cms_Quest);
		SERIALIZE_JSON(cms_Shop_BuyGold);
		SERIALIZE_JSON(cms_Shop_Package);
		SERIALIZE_JSON(cms_Shop_PackageDetail);
		SERIALIZE_JSON(cms_Support);
		SERIALIZE_JSON(cms_Support_Grade);
		SERIALIZE_JSON(cms_Support_StageSet);
		SERIALIZE_JSON(cms_Support_Value);
		SERIALIZE_JSON(cms_Table_Update);
		SERIALIZE_JSON(cms_TEXT_String);
		SERIALIZE_JSON(cms_UI_String);
		SERIALIZE_JSON(cms_Unlock_Condition);
		SERIALIZE_JSON(cms_UserLevel);
		SERIALIZE_JSON(cms_WeeklyRankingReward);
		SERIALIZE_JSON(cms_WeeklyRankingTier);
		SERIALIZE_JSON(cms_LoadingTip);
	}

	const CMS_Mode_Story* FindStoryById(int storyId) const
	{
		const CMS_Mode_Story* story = nullptr;
		for (const CMS_Mode_Story& itr : cms_Mode_Story)
		{
			if (itr.StoryNo == storyId)
			{
				story = &itr;
				break;
			}
		}
		return story;
	}

	const CMS_Item* FindItemById(const std::string& itemCode) const
	{
		const CMS_Item* item = nullptr;
		for (const CMS_Item& itr : cms_Item)
		{
			if (itr.ItemCode == itemCode)
			{
				item = &itr;
				break;
			}
		}
		return item;
	}
};