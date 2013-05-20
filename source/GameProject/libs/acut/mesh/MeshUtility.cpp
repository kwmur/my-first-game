#include "../acut.h"



namespace acut {


	/**
	 * @brief mesh��.x�t�@�C������ǂݍ���
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
			// �}�e���A���̃R�s�[
			meshMaterials.push_back(pMaterials[i].MatD3D);

			if (ambientEqualDiffuse) {
				// �A���r�G���g���f�B�t���[�Y�Ɠ����F�ɂ���
				meshMaterials.at(i).Ambient = meshMaterials.at(i).Diffuse;
			}

			meshTexturePointers.push_back(NULL);
			if (NULL != pMaterials[i].pTextureFilename
				&& 0 < lstrlenA(pMaterials[i].pTextureFilename)
			) {
				// texture�t�@�C���̃p�X���쐬
#ifdef UNICODE
				// pMaterials[i].pTextureFilename��UNICODE������֕ϊ�
				TCHAR textureFileName[MAX_PATH];
				MultiByteToWideChar(CP_ACP, 0, pMaterials[i].pTextureFilename, -1, textureFileName, MAX_PATH);
				textureFileName[MAX_PATH - 1] = _T('\0');
				acut::TString textureFilePath = meshFolderPath + acut::TString(textureFileName);
#else // #ifdef UNICODE
				acut::TString textureFilePath = meshFolderPath + acut::TString(pMaterials[i].pTextureFilename);
#endif // #ifdef UNICODE

				// �e�N�X�`���̍쐬
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

		// �����̃|�C���^�փZ�b�g
		*ppMesh = pMesh;
		*pNumberOfMaterials = numberOfMaterials;
		*pMeshMaterials = meshMaterials;
		*pMeshTexturePointers = meshTexturePointers;
	}


};
