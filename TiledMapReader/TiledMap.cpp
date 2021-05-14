#include "TiledMap.h"

namespace Upp{

TiledMapJson::TiledMapJson(const Upp::String& file){
	
}

TiledLayer::TiledLayer(const Value& jsonLayerObject){
	
}

TiledTilesSet::TiledTilesSet(const Value& jsonTilesSetObject){
		
}

TiledTilesSet::~TiledTilesSet(){}
int TiledTilesSet::GetFirstGID()const{return firstGid;}
const Upp::String TiledTilesSet::GetSource()const{return source;}
TiledLayer::~TiledLayer(){}
const Upp::String& TiledLayer::GetName()const{return name;}
const Upp::Vector<int>&  TiledLayer::GetDatas()const{return datas;}
int TiledLayer::GetId()const{return id;}
int TiledLayer::GetHeight()const{return height;}
int TiledLayer::GetWidth()const{return width;}
int TiledLayer::GetX()const{return x;}
int TiledLayer::GetY()const{return y;}
float TiledLayer::GetOpacity()const{return opacity;}
bool TiledLayer::IsVisible()const{return visible;}
TiledLayerType TiledLayer::GetType()const{return type;}
const ValueMap& TiledLayer::GetProperties()const{return properties;}
TiledMapJson::~TiledMapJson(){}
const Upp::String& TiledMapJson::GetVersion()const{return version;}
const Upp::String& TiledMapJson::GetTiledVersion()const{return tiledVersion;}
const Upp::Array<TiledLayer>& TiledMapJson::GetLayers()const{return layers;}
const Upp::Array<TiledTilesSet>& TiledMapJson::GetTilesSets()const{return tilesSets;}
TiledType TiledMapJson::GetTiledType()const{return type;}
TiledOrientation TiledMapJson::GetTiledOrientation()const{return orientation;}
TiledRenderOrder TiledMapJson::GetRenderOrder()const{return renderOrder;}
bool TiledMapJson::IsInfinite()const{return infinite;}
int TiledMapJson::GetCompressionLevel()const{return compressionLevel;}
int TiledMapJson::GetTileHeight()const{return tileHeight;}
int TiledMapJson::GetTileWidgth()const{return tileWidth;}
int TiledMapJson::GetWidth()const{return width;}
int TiledMapJson::GetHeight()const{return height;}
int TiledMapJson::GetNextLayerId()const{return nextLayerid;}
int TiledMapJson::GetNextObjectId()const{return nextObjectid;}

}