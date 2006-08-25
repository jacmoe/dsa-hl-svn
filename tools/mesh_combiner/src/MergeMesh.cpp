#include "MergeMesh.h"

#include <OgreMeshManager.h>
#include <OgreHardwareBufferManager.h>
#include <OgreSubmesh.h>

#include "MeshCombiner.h"


using namespace Ogre;

namespace rl {

    MergeMesh::MergeMesh() :
        m_BaseSkeleton(),
        m_Meshes()
	{
	}

	MergeMesh::~MergeMesh() 
	{    
        m_Meshes.clear();
        m_BaseSkeleton.setNull();
	}

    void MergeMesh::addMesh( Ogre::MeshPtr mesh )
	{    
        if( mesh->getSkeleton().isNull() )
        {
            MeshCombiner::getSingleton().log( "Skipped: " + mesh->getName() + " has no skeleton" );
            return;
        }

        if( m_BaseSkeleton.isNull() )
        {
            m_BaseSkeleton = mesh->getSkeleton();                    
            MeshCombiner::getSingleton().log( "Set: base skeleton (" + m_BaseSkeleton->getName()+")" );
        }

        if( mesh->getSkeleton() != m_BaseSkeleton )
        {
            MeshCombiner::getSingleton().log( "Skipped: " + mesh->getName() + " has other skeleton ("+ mesh->getSkeleton()->getName() +")" );
            return;
        }

        m_Meshes.push_back( mesh );
	}

    const String MergeMesh::findSubmeshName( MeshPtr m, ushort sid ) const
    {
        Mesh::SubMeshNameMap map = m->getSubMeshNameMap();
        for( Mesh::SubMeshNameMap::const_iterator it = map.begin();
             it != map.end(); ++it )
        {
            if( it->second == sid )
                return it->first;
        }

        return "";
    }   

    MeshPtr MergeMesh::bake()
    {    
         MeshCombiner::getSingleton().log( 
             "Baking: New Mesh started" );

        MeshPtr mp = MeshManager::getSingleton().
            createManual( "bakeMono", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
        mp->setSkeletonName( m_BaseSkeleton->getName() );

        AxisAlignedBox totalBounds = AxisAlignedBox();
        for( std::vector< Ogre::MeshPtr >::iterator it = m_Meshes.begin();
             it != m_Meshes.end(); ++it )
        {   
            MeshCombiner::getSingleton().log( 
                "Baking: adding submeshes for " + (*it)->getName() );

            // insert all submeshes
            for( ushort sid = 0; sid < (*it)->getNumSubMeshes(); ++sid )
            {
                SubMesh* sub = (*it)->getSubMesh( sid );
                const String name = findSubmeshName( (*it), sid );                
                
                // create submesh with correct name                
                SubMesh* newsub;
                if( name.length() == 0 )
                    newsub = mp->createSubMesh(  );
                else /// @todo check if a submesh with this name has been created before
                    newsub = mp->createSubMesh( name );   

                newsub->useSharedVertices = sub->useSharedVertices;

                // add index
                newsub->indexData = sub->indexData->clone();

                // add geometry
                if( !newsub->useSharedVertices )
                {
                    newsub->vertexData = sub->vertexData->clone();
                
                    // build bone assignments
                    SubMesh::BoneAssignmentIterator bit = sub->getBoneAssignmentIterator();
                    while( bit.hasMoreElements() )
                    {
                        VertexBoneAssignment vba = bit.getNext();
                        newsub->addBoneAssignment( vba );
                    }
                }

                newsub->setMaterialName( sub->getMaterialName() );

                MeshCombiner::getSingleton().log(  
                    "Baking: adding submesh '" + name + "'  with material " + sub->getMaterialName() );
            } 

            /// sharedvertices
            if( (*it)->sharedVertexData != NULL )
            {
                mp->sharedVertexData = (*it)->sharedVertexData->clone();

                Mesh::BoneAssignmentIterator bit = (*it)->getBoneAssignmentIterator();
                while( bit.hasMoreElements() )
                {
                    VertexBoneAssignment vba = bit.getNext();
                    mp->addBoneAssignment( vba );
                }
            }

            MeshCombiner::getSingleton().log( 
                "Baking: adding bounds for " + (*it)->getName() );

            // add bounds
            Vector3 oldmin = totalBounds.getMinimum();
            Vector3 newmin = (*it)->getBounds().getMinimum();
            Vector3 oldmax = totalBounds.getMaximum();
            Vector3 newmax = (*it)->getBounds().getMaximum();
            totalBounds = AxisAlignedBox( 
                std::min( oldmin.x, newmin.x ), std::min( oldmin.y, newmin.y  ), std::min( oldmin.z, newmin.z  ),
                std::max( oldmax.x, newmax.x ), std::max( oldmax.y, newmax.y  ), std::max( oldmax.z, newmax.z  )
                );
        }

        

        mp->_setBounds( totalBounds );

        /// @todo add parameters
        mp->buildEdgeList();

        MeshCombiner::getSingleton().log( 
            "Baking: Finished" );

        return mp;
	}

                /*
                bool use32BitIndexes = 
                    (sub->indexData->indexBuffer->getType() == HardwareIndexBuffer::IT_32BIT);
                newsub->operationType = sub->operationType;
                newsub->indexData->indexCount = sub->indexData->indexCount;
                
                HardwareIndexBufferSharedPtr newibuf = 
                    HardwareBufferManager::getSingleton().createIndexBuffer(
                        use32BitIndexes ? HardwareIndexBuffer::IT_32BIT : HardwareIndexBuffer::IT_16BIT, 
                        newsub->indexData->indexCount, 
                        HardwareBuffer::HBU_DYNAMIC,
                        false );
                newsub->indexData->indexBuffer = newibuf;
                HardwareIndexBufferSharedPtr oldibuf = sub->indexData->indexBuffer;
                unsigned int *pNewInt, *pOldInt;
                unsigned short *pNewShort, *pOldShort;
                if( use32BitIndexes )
                {
                    pNewInt = static_cast<unsigned int*>(newibuf->lock(HardwareBuffer::HBL_DISCARD) );
                    pOldInt = static_cast<unsigned int*>(oldibuf->lock(HardwareBuffer::HBL_READ_ONLY) );
                }
                else
                {
                    pNewShort = static_cast<unsigned short*>(newibuf->lock(HardwareBuffer::HBL_DISCARD) );
                    pOldShort = static_cast<unsigned short*>(oldibuf->lock(HardwareBuffer::HBL_READ_ONLY) );
                }
                for( unsigned int iid = 0; iid < newsub->indexData->indexCount; ++iid )
                {
                    if( use32BitIndexes )
                        *pNewInt++ = *pOldInt++;
                    else
                        *pNewShort++ = *pOldShort++;
                }
                newibuf->unlock();
                oldibuf->unlock();    
                        



                    new VertexData();                                                                  
                    newsub->vertexData->vertexCount = sub->vertexData->vertexCount;

                    if( newsub->vertexData->vertexCount <= 0 )
                        break;

                    VertexDeclaration* decl = newsub->vertexData->vertexDeclaration;
                    VertexBufferBinding* bind = newsub->vertexData->vertexBufferBinding;*/

}