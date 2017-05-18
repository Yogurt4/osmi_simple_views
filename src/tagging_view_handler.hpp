/*
 * tagging_view_handler.hpp
 *
 *  Created on:  2017-05-16
 *      Author: Michael Reichert <michael.reichert@geofabrik.de>
 */

#ifndef SRC_TAGGING_VIEW_HANDLER_HPP_
#define SRC_TAGGING_VIEW_HANDLER_HPP_

#include "abstract_view_handler.hpp"

class TaggingViewHandler : public AbstractViewHandler {
    static constexpr size_t MAX_STRING_LENGTH = 254;

    gdalcpp::Layer m_tagging_fixmes_on_nodes;
    gdalcpp::Layer m_tagging_fixmes_on_ways;
    gdalcpp::Layer m_tagging_nodes_with_empty_k;
    gdalcpp::Layer m_tagging_ways_with_empty_k;
    gdalcpp::Layer m_tagging_nodes_with_empty_v;
    gdalcpp::Layer m_tagging_ways_with_empty_v;
    gdalcpp::Layer m_tagging_misspelled_node_keys;
    gdalcpp::Layer m_tagging_misspelled_way_keys;

    /**
     * Write a feature to on of the layers which only have the fields
     * way_id/node_id, tag and lastchange.
     *
     * \param layer layer to write to
     * \param object OSM object
     * \param field_name name of the field which value should be written to.
     * If field_name is a nullptr, nothing will be written. Use `field_name=nullptr`
     * for layers which do not have any additional fields.
     * \param value value of tag field If field_name is a nullptr, nothing will
     * be written. Use `field_name=nullptr` for layers which do not have any
     * additional fields.
     */
    void write_feature_to_simple_layer(gdalcpp::Layer* layer,
            const osmium::OSMObject& object, const char* field_name, const char* value);

    /**
     * Write an object with a found spelling error to the output file.
     */
    void write_missspelled(const osmium::OSMObject& object,
            const char* key, const char* error, const char* otherkey);

    /**
     * Check if an object has one of the following keys: fixme=*, FIXME=* or todo=*.
     *
     * \param object object to be checked
     */
    void check_fixme(const osmium::OSMObject& object);

    /**
     * Check if an object has a key which contains whitespace.
     */
    void key_with_space(const osmium::OSMObject& object);

    /**
     * Check if an object has an empty key
     */
    void empty_key(const osmium::OSMObject& object);

    /**
     * Check if an object has an empty value
     */
    void empty_value(const osmium::OSMObject& object);

    /**
     * Check if an object has a tag with an unusual character
     */
    void unusual_character(const osmium::OSMObject& object);

    /**
     * Check if the length of a key is larger than 2 and smaller or equal than 50.
     */
    void check_key_length(const osmium::OSMObject& object);

    /**
     * Check if a character is an accepted character for keys and non-name values.
     */
    bool is_good_character(const char character);

    /**
     * Apply all checks on an object.
     */
    void handle_object(const osmium::OSMObject& object);

public:
    TaggingViewHandler() = delete;

    TaggingViewHandler(std::string& output_filename, std::string& output_format,
            osmium::util::VerboseOutput& verbose_output, int epsg = 3857);

    void node(const osmium::Node& node);

    void way(const osmium::Way& way);

    /**
     * Check if a key is a whitelisted key, e.g. "name", "short_name", "name:ru", description, description:en, comment, ….
     *
     * The substring name must be
     * * located at the beginning or preceeded by a colon or underscore
     * * and located at the end or followed by a colon or underscore
     *
     * This method will return wrong results if whitelisted_base occurs twice in key (e.g. named_name=*). But these cases are and the
     * common whitelisted keys name, description, note, comment and fixme don't have such stranges prefixes/suffixes.
     */
    static bool is_a_x_key_key(const char* key, const char* whitelisted_base);

    /**
     * Set some basic fields of a feature: ID, lastchange and one freely selectable field
     */
    static void set_basic_fields(gdalcpp::Feature& feature, const osmium::OSMObject& object,
            const char* field_name, const char* value);
};



#endif /* SRC_TAGGING_VIEW_HANDLER_HPP_ */