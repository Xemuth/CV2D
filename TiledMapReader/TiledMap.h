#ifndef _TiledMapReader_TiledMap_h_
#define _TiledMapReader_TiledMap_h_
#include <Core/core.h>


namespace Upp{

enum class TiledOrientation{Orthogonal, Isometrique, IsometriqueEchelonne, HexagonalEchelonne};
enum class TiledRenderOrder{RightDown, RightTop, LeftDown, LeftUp};
enum class TiledType{Map};
enum class TiledLayerType{TileLayer};

class TiledTilesSet{
	public:
		
	private:
		int firstGid;
		Upp::Stirng source;
};

class TiledLayer{
	public:
		
		
		
	private:
		Upp::Vector<int> datas;
		int height;
		int id;
		Upp::String name;
		float opacity;
		TiledLayerType type;
		bool visible;
		int width;
		int x;
		int y;
		ValueMap properties;
};

class TiledMapJson{
	public:
		TiledMapJson(const Upp::String& file);
		
		const Upp::String& GetVersion()const;
		const Upp::String& GetTiledVersion()const;
		
		const Upp::Array<TiledLayer>& GetLayers()const;
		const Upp::Array<TiledTilesSet>& GetTilesSets()const;
		
		TiledType GetTiledType()const;
		TiledOrientation GetTiledOrientation()const;
		TiledRenderOrder GetRenderOrder()const;
		
		bool IsInfinite()const;
		int GetCompressionLevel()const;
		int GetTileHeight()const;
		int GetTileWidgth()const;
		int GetWidth()const;
		int GetHeight()const;
		int GetNextLayerId()const;
		int GetNextObjectId()const;
	private:
		Upp::String version;
		Upp::String tiledversion;
		
		Upp::Array<TiledLayer> layers;
		Upp::Array<TiledTilesSet> tilessets
		
		int compressionlevel;
		bool infinite;
		
		TiledType type;
		TiledOrientation orientation;
		TiledRenderOrder renderorder;

		int tileheight;
		int tilewidth;
		
		int width;
		int height;
		
		int nextlayerid;
		int nextobjectid;
};

}

#endif
