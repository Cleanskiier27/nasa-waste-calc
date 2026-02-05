#pragma once

#include <memory>
#include <string>
#include <functional>
#include <vector>

namespace NRE {

/**
 * @brief Behavior tree for AI decision making
 * 
 * Hierarchical decision-making system for game AI
 */
class BehaviorTree {
public:
    enum class NodeType {
        Selector,       // OR: Choose first child that succeeds
        Sequence,       // AND: Execute children in order until one fails
        Parallel,       // Execute all children simultaneously
        Decorator,      // Modify child behavior
        Condition,      // Test a condition
        Action          // Perform an action
    };

    enum class Status {
        Success,
        Failure,
        Running
    };

    /**
     * @brief Base node class
     */
    class Node {
    public:
        virtual ~Node() = default;

        /**
         * @brief Execute node
         * @return Node status
         */
        virtual Status Execute() = 0;

        /**
         * @brief Reset node state
         */
        virtual void Reset() = 0;

        /**
         * @brief Get node type
         * @return Node type
         */
        virtual NodeType GetType() const = 0;
    };

    /**
     * @brief Selector node (OR logic)
     */
    class Selector : public Node {
    public:
        void AddChild(std::shared_ptr<Node> child);
        Status Execute() override;
        void Reset() override;
        NodeType GetType() const override { return NodeType::Selector; }

    private:
        std::vector<std::shared_ptr<Node>> m_Children;
        size_t m_CurrentChild = 0;
    };

    /**
     * @brief Sequence node (AND logic)
     */
    class Sequence : public Node {
    public:
        void AddChild(std::shared_ptr<Node> child);
        Status Execute() override;
        void Reset() override;
        NodeType GetType() const override { return NodeType::Sequence; }

    private:
        std::vector<std::shared_ptr<Node>> m_Children;
        size_t m_CurrentChild = 0;
    };

    /**
     * @brief Condition node (test)
     */
    class Condition : public Node {
    public:
        using ConditionFunc = std::function<bool()>;

        Condition(ConditionFunc func) : m_Func(func) {}
        Status Execute() override;
        void Reset() override {}
        NodeType GetType() const override { return NodeType::Condition; }

    private:
        ConditionFunc m_Func;
    };

    /**
     * @brief Action node (do something)
     */
    class Action : public Node {
    public:
        using ActionFunc = std::function<Status()>;

        Action(ActionFunc func) : m_Func(func) {}
        Status Execute() override;
        void Reset() override {}
        NodeType GetType() const override { return NodeType::Action; }

    private:
        ActionFunc m_Func;
    };

    /**
     * @brief Create behavior tree
     * @param root Root node
     * @return Unique pointer to behavior tree
     */
    static std::unique_ptr<BehaviorTree> Create(std::shared_ptr<Node> root);

    virtual ~BehaviorTree() = default;

    /**
     * @brief Execute tree (call each frame)
     * @return Tree status
     */
    virtual Status Execute() = 0;

    /**
     * @brief Reset tree to initial state
     */
    virtual void Reset() = 0;

    /**
     * @brief Get root node
     * @return Root node
     */
    virtual std::shared_ptr<Node> GetRoot() const = 0;
};

} // namespace NRE
