#include "texturing.h"

void
build_adjacency_graph(UniGraph * graph, mve::TriangleMesh::ConstPtr mesh,
    mve::VertexInfoList::ConstPtr vertex_infos) {

    mve::TriangleMesh::FaceList const & faces = mesh->get_faces();
    std::size_t const num_faces = faces.size() / 3;

    ProgressCounter face_counter("\tAdding edges", num_faces);
    for (std::size_t i = 0; i < faces.size(); i += 3) {
        face_counter.progress<SIMPLE>();

        std::size_t v1 = faces[i];
        std::size_t v2 = faces[i + 1];
        std::size_t v3 = faces[i + 2];

        std::vector<std::size_t> adj_faces;
        vertex_infos->get_faces_for_edge(v1, v2, &adj_faces);
        vertex_infos->get_faces_for_edge(v2, v3, &adj_faces);
        vertex_infos->get_faces_for_edge(v3, v1, &adj_faces);

        for (std::size_t j = 0; j < adj_faces.size(); ++j) {
            /* Face id vs. face position. */
            std::size_t face = i / 3;
            std::size_t adj_face = adj_faces[j];

            /* Avoid self referencing. */
            if (face != adj_face) {
                /* Edge not already in graph? */
                if (!graph->has_edge(face, adj_face)){
                    graph->add_edge(face, adj_face);
                }
            }
        }
        face_counter.inc();
    }

    std::cout << "\t" << graph->num_edges() << " total edges." << std::endl;
}
