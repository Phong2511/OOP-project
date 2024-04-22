#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Voronoi_diagram_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point_2;
typedef CGAL::Voronoi_diagram_2<K> VD;
typedef VD::Face_handle Face_handle;
typedef VD::Ccb_halfedge_circulator Ccb_halfedge_circulator;

class Pedestrian {
    //Các phương thức khác

public:
    std::vector<Point_2> getIntermediatePoints() const {
        std::vector<Point_2> intermediatePoints;

        // Tạo một đối tượng Voronoi diagram
        VD vd;

        // Thêm các điểm của hành trình vào diagram
        for (const auto& ward : journey) {
            vd.insert(Point_2(ward.getX(), ward.getY()));
        }

        // Duyệt qua các cell của diagram để tìm điểm trung gian
        for (auto it = vd.faces_begin(); it != vd.faces_end(); ++it) {
            Face_handle face = it;
            Ccb_halfedge_circulator circulator = face->outer_ccb();

            // Tính toán trung điểm của các cạnh của cell
            Point_2 center(0, 0);
            int count = 0;
            do {
                Point_2 source = circulator->source()->point();
                Point_2 target = circulator->target()->point();
                center = center + (source + target) / 2;
                ++count;
            } while (++circulator != face->outer_ccb());

            if (count > 0) {
                center = center / count;
                intermediatePoints.push_back(center);
            }
        }

        return intermediatePoints;
    }
};
