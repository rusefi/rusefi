<?php
/**
 *
 * @package Quick Title Edition Extension
 * @copyright (c) 2015 ABDev
 * @copyright (c) 2015 PastisD
 * @copyright (c) 2015 Geolim4 <http://geolim4.com>
 * @copyright (c) 2015 Zoddo <zoddo.ino@gmail.com>
 * @license http://opensource.org/licenses/gpl-2.0.php GNU General Public License v2
 *
 */

namespace ernadoo\qte\event;

use Symfony\Component\EventDispatcher\EventSubscriberInterface;

class main_listener implements EventSubscriberInterface
{
	/** @var \phpbb\request\request */
	protected $request;

	/** @var \phpbb\template\template */
	protected $template;

	/** @var \phpbb\user */
	protected $user;

	/** @var \phpbb\log\log */
	protected $log;

	/** @var \ernadoo\qte\qte */
	protected $qte;

	public function __construct(\phpbb\request\request $request, \phpbb\template\template $template, \phpbb\user $user, \phpbb\log\log $log, \ernadoo\qte\qte $qte)
	{
		$this->request	= $request;
		$this->template	= $template;
		$this->user		= $user;
		$this->log		= $log;
		$this->qte		= $qte;
	}

	static public function getSubscribedEvents()
	{
		return array(
			// viewforum
			'core.viewforum_modify_topics_data'	=> 'viewforum_get_user_infos',
			'core.viewforum_modify_topicrow'	=> 'viewforum_assign_topic_attributes',

			// viewtopic
			'core.viewtopic_add_quickmod_option_before'		=> 'viewtopic_attr_apply',
			'core.viewtopic_assign_template_vars_before'	=> 'viewtopic_select_assign_attributes',
			'core.viewtopic_modify_page_title'				=> 'viewtopic_attr_title',

			// posting
			'core.posting_modify_template_vars'			=> 'posting_select_assign_attributes',
			'core.posting_modify_submission_errors'		=> 'posting_check_attribute',
			'core.posting_modify_submit_post_before'	=> 'posting_submit_data',
			'core.submit_post_modify_sql_data'			=> 'posting_save_attribute',
		);
	}

	public function viewforum_assign_topic_attributes($event)
	{
		if (!empty($event['row']['topic_attr_id']))
		{
			$topic_row = $event['topic_row'];
			$topic_row['TOPIC_ATTRIBUTE'] = $this->qte->attr_display($event['row']['topic_attr_id'], $event['row']['topic_attr_user'], $event['row']['topic_attr_time']);
			$event['topic_row'] = $topic_row;
		}
	}

	public function viewforum_get_user_infos($event)
	{
		if (sizeof($event['topic_list']))
		{
			$this->qte->get_users_by_topic_id($event['topic_list']);
		}
	}

	public function viewtopic_attr_title($event)
	{
		$attr_title = $this->qte->attr_title($event['topic_data']['topic_attr_id'], $event['topic_data']['topic_attr_user'], $event['topic_data']['topic_attr_time']);
		$event['page_title'] = $attr_title ? $attr_title . ' ' . $event['page_title'] : $event['page_title'];
	}

	public function viewtopic_select_assign_attributes($event)
	{
		if (!empty($event['topic_data']['topic_attr_id']))
		{
			$this->qte->get_users_by_topic_id(array($event['topic_data']['topic_id']));
			$this->template->assign_var('TOPIC_ATTRIBUTE', $this->qte->attr_display($event['topic_data']['topic_attr_id'], $event['topic_data']['topic_attr_user'], $event['topic_data']['topic_attr_time']));
		}

		$this->qte->attr_select($event['forum_id'], $event['topic_data']['topic_poster'], (int) $event['topic_data']['topic_attr_id'], $event['viewtopic_url']);
	}

	public function viewtopic_attr_apply($event)
	{
		$attr_id = (int) $this->request->variable('attr_id', 0);
		if ($attr_id)
		{
			$this->qte->get_users_by_user_id($this->user->data['user_id']);
			$this->qte->attr_apply($attr_id, $event['topic_id'], $event['forum_id'], $event['topic_data']['topic_attr_id'], $event['topic_data']['topic_poster'], $event['viewtopic_url']);
		}
	}

	public function posting_select_assign_attributes($event)
	{
		$topic_attribute = $this->request->variable('attr_id', !empty($event['post_data']['topic_attr_id']) ? \ernadoo\qte\qte::KEEP : 0, false, \phpbb\request\request_interface::POST);

# old line		$this->qte->attr_select($event['forum_id'], !empty($event['post_data']['topic_attr_user']) ? $event['post_data']['topic_attr_user'] : 0, (int) $topic_attribute, '', $event['mode']);
# custom version see https://github.com/rusefi/rusefi/issues/556
			$this->qte->attr_select($event['forum_id'], !empty($event['post_data']['topic_poster']) ? $event['post_data']['topic_poster'] : 0, (int) $topic_attribute, '', $event['mode']);

		if ($event['mode'] != 'post')
		{
			$post_data = $event['post_data'];

			if ($topic_attribute != \ernadoo\qte\qte::KEEP)
			{
				$post_data['topic_attr_id']		= (int) $topic_attribute;
				$post_data['topic_attr_user']	= (int) $this->user->data['user_id'];
				$post_data['topic_attr_time']	= time();

				$this->qte->get_users_by_user_id($this->user->data['user_id']);
			}

			if ($topic_attribute != \ernadoo\qte\qte::REMOVE)
			{
				$this->qte->get_users_by_topic_id(array($post_data['topic_id']));
				$this->template->assign_var('TOPIC_ATTRIBUTE', $this->qte->attr_display($post_data['topic_attr_id'], $post_data['topic_attr_user'], $post_data['topic_attr_time']));
			}
		}
	}

	public function posting_check_attribute($event)
	{
		$post_data = $event['post_data'];
		$post_data['attr_id'] = $this->request->variable('attr_id', \ernadoo\qte\qte::KEEP, false, \phpbb\request\request_interface::POST);

		if ($event['post_data']['force_attr'])
		{
			if ($post_data['attr_id'] == \ernadoo\qte\qte::REMOVE && ($event['mode'] == 'post' || ($event['mode'] == 'edit' && $event['post_data']['topic_first_post_id'] == $event['post_id'])) )
			{
				$error = $event['error'];
				$error[] = $this->user->lang['QTE_ATTRIBUTE_UNSELECTED'];
				$event['error'] = $error ;

				// init the value
				$post_data['attr_id'] = 0;
			}
		}

		$event['post_data'] = $post_data;
	}

	public function posting_submit_data($event)
	{
		$topic_attribute = $event['post_data']['attr_id'];

		if ($event['mode'] != 'post' && $topic_attribute == $event['post_data']['topic_attr_id'])
		{
			$topic_attribute = \ernadoo\qte\qte::KEEP;
		}
		else if (empty($event['post_data']['topic_attr_id']) && $topic_attribute == \ernadoo\qte\qte::REMOVE)
		{
			$topic_attribute = \ernadoo\qte\qte::KEEP;
		}

		$data = $event['data'];
		$data['attr_id'] = (int) (($event['mode'] == 'post') && !empty($event['post_data']['default_attr'])) ? $event['post_data']['default_attr'] : $topic_attribute;
		$event['data'] = $data;
	}

	public function posting_save_attribute($event)
	{
		if (isset($event['data']['attr_id']) && $event['data']['attr_id'] != \ernadoo\qte\qte::KEEP)
		{
			$sql_data = $event['sql_data'];
			if ($event['data']['attr_id'] == \ernadoo\qte\qte::REMOVE)
			{
				$sql_data[TOPICS_TABLE]['stat'][] = 'topic_attr_id = 0';
				$sql_data[TOPICS_TABLE]['stat'][] = 'topic_attr_user = 0';
				$sql_data[TOPICS_TABLE]['stat'][] = 'topic_attr_time = 0';
			}
			else
			{
				$sql_data[TOPICS_TABLE]['stat'][] = 'topic_attr_id = ' . (int) $event['data']['attr_id'];
				$sql_data[TOPICS_TABLE]['stat'][] = 'topic_attr_user = ' . (int) $this->user->data['user_id'];
				$sql_data[TOPICS_TABLE]['stat'][] = 'topic_attr_time = ' . time();
			}
			$event['sql_data'] = $sql_data;

			if (in_array($event['post_mode'], array('edit_topic', 'edit_first_post')))
			{
				$attr_name = ($event['data']['attr_id'] != \ernadoo\qte\qte::REMOVE) ? $this->qte->get_attr_name_by_id($event['data']['attr_id']) : '';
				$log_data = array(
					'forum_id' => $event['data']['forum_id'],
					'topic_id' => $event['data']['topic_id'],
					$attr_name,
				);

				$this->log->add('mod', $this->user->data['user_id'], $this->user->ip, 'MCP_ATTRIBUTE_' . ($event['data']['attr_id'] == \ernadoo\qte\qte::REMOVE ? 'REMOVED' : 'UPDATED'), time(), $log_data);
			}
		}
	}
}
