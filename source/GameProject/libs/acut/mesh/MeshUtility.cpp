#include "../acut.h"



namespace acut {


	/**
	 * @brief meshを.xファイルから読み込む
	 *
	 */
	void MeshUtility::loadMeshFromX(
		LPD3DXMESH* ppMesh,
		DWORD* pNumberOfMaterials,
		std::vector<D3DMATERIAL9>* pMeshMaterials,
		std::vector<LPDIRECT3DTEXTURE9>* pMeshTexturePointers,
		const acut::TString& meshFilePath,
		const acut::TString& meshFolderPath,
		const bool ambientEqualDiffuse
	)
	{
		LPD3DXMESH pMesh = 0;
		DWORD numberOfMaterials = 0;
		LPD3DXBUFFER pBuffer = 0;

		V_THROW(D3DXLoadMeshFromX(
			meshFilePath.c_str(),
			D3DXMESH_MANAGED,
			DXUTGetD3D9Device(),
			NULL,
			&pBuffer,
			NULL,
			&numberOfMaterials,
			&pMesh
		));

		std::vector<D3DMATERIAL9> meshMaterials;
		std::vector<LPDIRECT3DTEXTURE9> meshTexturePointers;
		LPD3DXMATERIAL pMaterials = static_cast<LPD3DXMATERIAL>(pBuffer->GetBufferPointer());

		for (DWORD i = 0; i < numberOfMaterials; ++i) {
			// マテリアルのコピー
			meshMaterials.push_back(pMaterials[i].MatD3D);

			if (ambientEqualDiffuse) {
				// アンビエントをディフューズと同じ色にする
				meshMaterials.at(i).Ambient = meshMaterials.at(i).Diffuse;
			}

			meshTexturePointers.push_back(NULL);
			if (NULL != pMaterials[i].pTextureFilename
				&& 0 < lstrlenA(pMaterials[i].pTextureFilename)
			) {
				// textureファイルのパスを作成
#ifdef UNICODE
				// pMaterials[i].pTextureFilenameをUNICODE文字列へ変換
				TCHAR textureFileName[MAX_PATH];
				MultiByteToWideChar(CP_ACP, 0, pMaterials[i].pTextureFilename, -1, textureFileName, MAX_PATH);
				textureFileName[MAX_PATH - 1] = _T('\0');
				acut::TString textureFilePath = meshFolderPath + acut::TString(textureFileName);
#else // #ifdef UNICODE
				acut::TString textureFilePath = meshFolderPath + acut::TString(pMaterials[i].pTextureFilename);
#endif // #ifdef UNICODE

				// テクスチャの作成
				if (FAILED(D3DXCreateTextureFromFile(
					DXUTGetD3D9Device(),
					textureFilePath.c_str(),
					&meshTexturePointers.at(i)
				))) {
					MessageBox(NULL, L"Could not find texture map", L"Meshes.exe", MB_OK);
				}
			}
		}

		pBuffer->Release();

		// 引数のポインタへセット
		*ppMesh = pMesh;
		*pNumberOfMaterials = numberOfMaterials;
		*pMeshMaterials = meshMaterials;
		*pMeshTexturePointers = meshTexturePointers;
	}


};
